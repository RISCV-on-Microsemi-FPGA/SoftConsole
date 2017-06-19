/*
    FreeRTOS V8.2.3 - Copyright (C) 2015 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution and was contributed
    to the project by Technolution B.V. (www.technolution.nl,
    freertos-riscv@technolution.eu) under the terms of the FreeRTOS
    contributors license.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

/*
 * main() creates a set of standard demo task and a timer.
 * It then starts the scheduler.  The web documentation provides
 * more details of the standard demo application tasks, which provide no
 * particular functionality, but do provide a good example of how to use the
 * FreeRTOS API.
 *
 *
 * In addition to the standard demo tasks, the following tasks and timer are
 * defined and/or created within this file:
 *
 * "Check" software timer - The check timer period is initially set to three
 * seconds.  Its callback function checks that all the standard demo tasks, and
 * the register check tasks, are not only still executing, but are executing
 * without reporting any errors.
 */

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#include "riscv_hal.h"
#include "hw_platform.h"
#include "core_uart_apb.h"
#include "core_gpio.h"

#include "task.h"

/* lwIP includes. */
#include "lwip/tcpip.h"
#include "lwip/dhcp.h"

#include "drivers/CoreTSE/core_tse.h"
#include "ethernet_status.h"

const char * g_hello_msg = "\r\n	/*CoreTSE WebServer using RISC-V Soft Processor*/	\r\n";

UART_instance_t g_uart;

/*-----------------------------------------------------------------------------
 * GPIO instance data.
 */

gpio_instance_t g_gpio_in;
gpio_instance_t g_gpio_out;

/* Priorities used by the various different tasks. */
#define HTTPD_TASK_PRIORITY                   	1
#define LINK_STATUS_TASK_PRIORITY            	1
#define uartPRIMARY_PRIORITY					1



/* The period after which the check timer will expire provided no errors have
been reported by any of the standard demo tasks.  ms are converted to the
equivalent in ticks using the portTICK_PERIOD_MS constant. */
#define mainCHECK_TIMER_PERIOD_MS			( 3000UL / portTICK_PERIOD_MS )

/* A block time of zero simply means "don't block". */
#define mainDONT_BLOCK						( 0UL )

/* Web server task stack size. */
#define HTTPD_STACK_SIZE                        2048
#define LINK_STATUS_TASK_STACK_SIZE             1024


#define ETHERNET_STATUS_QUEUE_LENGTH    1
#define DONT_BLOCK                      0

/*-----------------------------------------------------------*/
void http_server_netconn_thread(void *arg);
void prvLinkRXTask(void * pvParameters);

void prvLinkStatusTask(void * pvParameters);
/*
 * Ethernet interface configuration function.
 */
static void prvEthernetConfigureInterface(void * param);
/*
 * FreeRTOS hook for when malloc fails, enable in FreeRTOSConfig.
 */
void vApplicationMallocFailedHook( void );

/*
 * FreeRTOS hook for when freertos is idling, enable in FreeRTOSConfig.
 */
void vApplicationIdleHook( void );

/*
 * FreeRTOS hook for when a stackoverflow occurs, enable in FreeRTOSConfig.
 */
void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName );



/*==============================================================================
 *
 */
/* The queue used by PTPd task to trnsmit status information to webserver task. */
xQueueHandle xPTPdOutQueue = NULL;


/* lwIP MAC configuration. */
static struct netif s_EMAC_if;

/*-----------------------------------------------------------*/

void get_mac_address(uint8_t * mac_addr)
{
    uint32_t inc;

    for(inc = 0; inc < 6; ++inc)
    {
        mac_addr[inc] = s_EMAC_if.hwaddr[inc];
    }
}

uint32_t get_ip_address(void)
{
    return (uint32_t)(s_EMAC_if.ip_addr.addr);
}
extern QueueHandle_t xRXQueue;

int main( void )
{
    /*
     * Create the queue used by the RX interrupt and prvLinkRXTask task to synchrinize frame reception
     */
	xRXQueue = xQueueCreate( 10, sizeof( uint32_t ) );

    if( xRXQueue == NULL )
    {
       /* Queue was not created and must not be used. */
    }

	UART_init( &g_uart, COREUARTAPB0_BASE_ADDR, BAUD_VALUE_115200, (DATA_8_BITS | NO_PARITY));
    UART_polled_tx_string(&g_uart, (const uint8_t *)g_hello_msg);


    /* Create the web server task. */
    tcpip_init(prvEthernetConfigureInterface, NULL);


    xTaskCreate( prvLinkRXTask, "LinkRX", 1024, NULL, uartPRIMARY_PRIORITY, NULL );

    xTaskCreate(http_server_netconn_thread,
                (signed char *) "http_server",
                HTTPD_STACK_SIZE,
                NULL,
				HTTPD_TASK_PRIORITY,
                NULL );

    /* Create the task the Ethernet link status. */
    xTaskCreate(prvLinkStatusTask,
                (signed char *) "EthLinkStatus",
                LINK_STATUS_TASK_STACK_SIZE,
                NULL,
				LINK_STATUS_TASK_PRIORITY,
                NULL);

    PLIC_init();

    /* Start the kernel.  From here on, only tasks and interrupts will run. */
	vTaskStartScheduler();

	 /*
     * If all is well, the scheduler will now be running, and the following line
     * will never be reached.  If the following line does execute, then there
     * was insufficient FreeRTOS heap memory available for the idle and/or timer
     * tasks to be created.  See the memory management section on the FreeRTOS
     * web site for more details.
     */
	 /* Exit FreeRTOS */
	return 0;
}
/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
	/* vApplicationMallocFailedHook() will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	function that will get called if a call to pvPortMalloc() fails.
	pvPortMalloc() is called internally by the kernel whenever a task, queue,
	timer or semaphore is created.  It is also called by various parts of the
	demo application.  If heap_1.c or heap_2.c are used, then the size of the
	heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
	FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
	to query the size of free heap space that remains (although it does not
	provide information on how the remaining heap might be fragmented). */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
    volatile size_t xFreeStackSpace;

    /*
     * This function is called on each cycle of the idle task.  In this case it
     * does nothing useful, other than report the amount of FreeRTOS heap that
     * remains unallocated.
     */
    xFreeStackSpace = xPortGetFreeHeapSize();

    if( xFreeStackSpace > 100 )
    {
        /*
         * By now, the kernel has allocated everything it is going to, so if
         * there is a lot of heap remaining unallocated then the value of
         * configTOTAL_HEAP_SIZE in FreeRTOSConfig.h can be reduced accordingly.
         */
    }

}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/

static void prvEthernetConfigureInterface(void * param)
{
    struct ip_addr xIpAddr, xNetMast, xGateway;
    extern err_t ethernetif_init( struct netif *netif );

    /* Parameters are not used - suppress compiler error. */
    ( void ) param;

    /* Create and configure the EMAC interface. */
#ifdef NET_USE_DHCP
    IP4_ADDR( &xIpAddr, 0, 0, 0, 0 );
    IP4_ADDR( &xGateway, 192, 168, 1, 254 );
#else
    IP4_ADDR( &xIpAddr, 169, 254, 1, 23 );
    IP4_ADDR( &xGateway, 169, 254, 1, 23 );
#endif

    IP4_ADDR( &xNetMast, 255, 255, 255, 0 );

    netif_add( &s_EMAC_if, &xIpAddr, &xNetMast, &xGateway, NULL, ethernetif_init, tcpip_input );

    /* bring it up */

#ifdef NET_USE_DHCP
    dhcp_start(&s_EMAC_if);
#else
    netif_set_up(&s_EMAC_if);
#endif

    /* make it the default interface */
    netif_set_default(&s_EMAC_if);
}

uint8_t buf[50]={0};
uint32_t ip_addr;
uint16_t address[4];

void prvLinkStatusTask(void * pvParameters)
{
    ethernet_status_t status;
    static uint8_t acquired = 0;

    status.speed = TSE_MAC10MBPS;
    status.duplex_mode = TSE_FULL_DUPLEX;
    for(;;)
    {
        volatile uint8_t linkup;
        uint8_t fullduplex;
        tse_speed_t speed;

        /* Run through loop every 5seconds. */
        vTaskDelay(50 / portTICK_RATE_MS);

        if(0 == acquired)
        {
			ip_addr = get_ip_address();
			address[0] = ((uint16_t)((ip_addr >> 24u) & 0x000000FFu));
			address[1] = ((int)((ip_addr >> 16u) & 0x000000FFu));
			address[2] = ((int)((ip_addr >> 8u) & 0x000000FFu));
			address[3] = ((int)((ip_addr ) & 0x000000FFu));

			sprintf(buf, "\n\r %d.%d.%d.%d\n\r",address[3],address[2],address[1],address[0]);

			UART_polled_tx_string(&g_uart, (const uint8_t *)buf);
        }

        if(0 != ip_addr)
        	acquired = 1;
    }
}


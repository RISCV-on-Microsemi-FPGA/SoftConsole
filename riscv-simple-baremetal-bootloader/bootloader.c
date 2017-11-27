/*******************************************************************************
 * (c) Copyright 2016-2017 Microsemi SoC Products Group. All rights reserved.
 *
 *  Simple boot-loader example program.
 *  This sample project is targeted at a RISC-V design running on the M2S150
 *  development board.
 *  You can program the SPI Flash from a command line program and have the
 *  boot-loader load a program from SPI Flash and jump to it.
 *  These actions are driven from a serial command line interface.
 *
 * SVN $Revision: $
 * SVN $Date: $
 */
#include <unistd.h>
#include <string.h>

#include "hw_platform.h"
#include "riscv_hal.h"

#include "core_gpio.h"
#include "core_spi.h"
#include "core_timer.h"
#include "core_uart_apb.h"

#include "spi_flash.h"
#include "ymodem.h"


#define FLASH_SECTOR_SIZE   65536 // Sectors are 64K bytes
#define FLASH_SECTORS       128   // This is an 8MB part with 128 sectors of 64KB
#define FLASH_BLOCK_SIZE    4096  // We will use the 4K blocks for this example
#define FLASH_SEGMENT_SIZE  256   // Write segment size

#define FLASH_BLOCK_SEGMENTS ( FLASH_BLOCK_SIZE / FLASH_SEGMENT_SIZE )

#define FLASH_BYTE_SIZE		(FLASH_SECTOR_SIZE * FLASH_SECTORS)
#define LAST_BLOCK_ADDR     (FLASH_BYTE_SIZE - FLASH_BLOCK_SIZE)

static int test_flash(void);
static void mem_test(uint8_t *address);
static uint32_t rx_app_file(uint8_t *dest_address);
static void Bootloader_JumpToApplication(uint32_t stack_location, uint32_t reset_vector);
static int read_program_from_flash(uint8_t *read_buf, uint32_t read_byte_length);
static int write_program_to_flash(uint8_t *write_buf, uint32_t file_size);
static void show_progress(void);

/*
 * Data structure stored at the beginning of SPI flash to indicate the suize of
 * data stored inside the SPI flash. This is to avoid having to read the entire
 * flash content at boot time.
 * This data structure is one flash segment long (256 bytes).
 */
typedef struct
{
    uint32_t validity_key;
    uint32_t spi_content_byte_size;
    uint32_t dummy[62];
} flash_content_t;

/*
 * Base address of DDR memory where executable will be loaded.
 */
#define DDR_BASE_ADDRESS    0x80000000
 
/*
 * Delay loop down counter load value.
 */
#define DELAY_LOAD_VALUE     0x00008000

/*
 * Bit mask identifying the DIP switch used to indicate whether the boot loader
 * should load and launch the application on system reset or stay running to
 * allow a new image to be programming into the SPI flash.
 */
#define BOOTLOADER_DIP_SWITCH   0x00000080

/*
 * Key value used to determine if valid data is contained in the SPI flash.
 */
#define SPI_FLASH_VALID_CONTENT_KEY     0xB5006BB1

/*
 * CoreGPIO instance data.
 */
gpio_instance_t g_gpio;

volatile uint32_t g_10ms_count;
volatile uint32_t g_state;


/******************************************************************************
 * Maximum receiver buffer size.
 *****************************************************************************/
#define MAX_RX_DATA_SIZE    256

/******************************************************************************
 * CoreUARTapb instance data.
 *****************************************************************************/
UART_instance_t g_uart;

/******************************************************************************
 * Instruction message. This message will be transmitted over the UART to
 * HyperTerminal when the program starts.
 *****************************************************************************/
const uint8_t g_greeting_msg[] =
"\r\n\r\n\
===============================================================================\r\n\
                    Microsemi RISC-V Boot Loader v0.2.2\r\n\
===============================================================================\r\n\
 This boot loader provides the following features:\r\n\
    - Load a program into DDR memory using the YModem file transfer protocol.\r\n\
    - Write a program into the board's SPI flash. The executable must first be\r\n\
      loaded into the board's DDR memory using the YModem file transfer\r\n\
      protocol.\r\n\
    - Load a program from SPI flash into external DDR memory and launch the\r\n\
      program.\r\n\
    - Automatically load and execute a program from SPI flash depending on DIP\r\n\
      switch 0 position.\r\n\
";

const uint8_t g_instructions_msg[] =
"\r\n\r\n\
-------------------------------------------------------------------------------\r\n\
 Type 0 to show this menu\n\r\
 Type 1 to start Ymodem transfer to DDR memory\n\r\
 Type 2 to copy program from DDR to flash \n\r\
 Type 3 to copy program from flash to DDR \n\r\
 Type 4 to start program loaded in DDR \n\r\
 Type 5 to test Flash device 0\n\r\
 Type 6 to test DDR\n\r\
";

const uint8_t g_boot_dip_switch_off_msg[] =
"\r\n\
-------------------------------------------------------------------------------\r\n\
 Boot loader jumper/switch set to start application.\r\n\
 Toggle DIP switch 0 and reset the board if you wish to execute the boot loader\r\n\
 to load another application into the SPI flash.\r\n\
";

const uint8_t g_boot_dip_switch_on_msg[] =
"\r\n\
-------------------------------------------------------------------------------\r\n\
 Boot loader jumper/switch set to stay in boot loader.\r\n\
 Toggle DIP switch 0 and reset the board if you wish to automatically execute\r\n\
 the application stored in the SPI flash on reset.\r\n\
";

const uint8_t g_load_executable_msg[] =
"\r\n\
-------------------------------------------------------------------------------\r\n\
 Loading application from SPI flash into DDR memory.\r\n";

const uint8_t g_run_executable_msg[] =
"\r\n\
-------------------------------------------------------------------------------\r\n\
 Executing application in DDR memory.\r\n\
-------------------------------------------------------------------------------\r\n\
 \r\n";

/******************************************************************************
 * Timer load value. This value is calculated to result in the timer timing
 * out after after 1 second with a system clock of 24MHz and the timer
 * prescaler set to divide by 1024.
 *****************************************************************************/
#define TIMER_LOAD_VALUE    23437

/******************************************************************************
 * GPIO instance data.
 *****************************************************************************/
gpio_instance_t g_gpio;

/*-------------------------------------------------------------------------*//**
 * main() function.
 */
int main()
{
	uint8_t rx_data[MAX_RX_DATA_SIZE];
	size_t rx_size;
	char wait_in_bl;

    g_10ms_count = 0;
    
    /**************************************************************************
     * Initialize the RISC-V platform level interrupt controller. 
     *************************************************************************/
    PLIC_init();
    
    /**************************************************************************
     * Initialize the CoreGPIO driver with the base address of the CoreGPIO
     * instance to use and the initial state of the outputs.
     *************************************************************************/
    GPIO_init( &g_gpio, COREGPIO_BASE_ADDR, GPIO_APB_32_BITS_BUS );

    /**************************************************************************
     * Configure the GPIOs.
     *************************************************************************/
	GPIO_set_output( &g_gpio, GPIO_0, 0x00 );
	GPIO_set_output( &g_gpio, GPIO_1, 0x00 );

    /**************************************************************************
      * Initialize CoreUARTapb with its base address, baud value, and line
      * configuration.
      *************************************************************************/
	UART_init( &g_uart, COREUARTAPB0_BASE_ADDR,\
			BAUD_VALUE_115200, (DATA_8_BITS | NO_PARITY) );

    /**************************************************************************
     * Display greeting message message.
     *************************************************************************/
	UART_polled_tx_string( &g_uart, g_greeting_msg);

    /**************************************************************************
     * Set up the system tick timer
     *************************************************************************/
    SysTick_Config(SYS_CLK_FREQ / 100);

   	/*
	 * Check to see if boot-loader switch is set
	 */
	if (GPIO_get_inputs( &g_gpio) & BOOTLOADER_DIP_SWITCH)
	{
		wait_in_bl = 1;
        UART_polled_tx_string( &g_uart, g_boot_dip_switch_on_msg);
        UART_polled_tx_string( &g_uart, g_instructions_msg);
	}
	else
	{
		wait_in_bl = 0;
        UART_polled_tx_string( &g_uart, g_boot_dip_switch_off_msg);
	}

    while(wait_in_bl == 1)
    {
    	static uint32_t file_size = 0;
    	static uint32_t readback_size = (126 * 1024) /*FLASH_BYTE_SIZE*/;

         /**********************************************************************
         * Read data received by the UART.
         *********************************************************************/
        rx_size = UART_get_rx( &g_uart, rx_data, sizeof(rx_data) );


        /**********************************************************************
         * Echo back data received, if any.
         *********************************************************************/
        if ( rx_size > 0 )
        {
            UART_send( &g_uart, rx_data, rx_size );

            switch(rx_data[0])
            {
                case '0':
                    UART_polled_tx_string( &g_uart, g_instructions_msg);
                    break;
                case '1':
                	file_size = rx_app_file((uint8_t *)DDR_BASE_ADDRESS);
                    break;
                case '2':
                    write_program_to_flash((uint8_t *)DDR_BASE_ADDRESS, file_size);
                    readback_size = file_size;
                    break;
                case '3':
                    read_program_from_flash((uint8_t *)DDR_BASE_ADDRESS, readback_size);
                    break;
                case '4':
                	/* Populate with stack and reset vector address i.e. The first two words of the program */
                    Bootloader_JumpToApplication((uint32_t)0x60000000, (uint32_t)0x60000004);
                    break;
                case '5':
                    /* Works- but needed to blow open Libero "RTG4 DDR Memory controller with initialization"
                     * and edit with parameters taken from design on web site- see ABC program below */
                    test_flash();
                    break;
                case '6':
                    /*
                     * Sanity check on DDR working
                     */
                    mem_test((uint8_t *)DDR_BASE_ADDRESS);
                    break;
                case '7':
					/* test soft reset- with CoreBootStrap */
                	/* wipe reset vector */
                	/* choose this option */
                	/* system should reboot correctly */
//<CJ>                	__disable_irq();
//<CJ>                	*acr = 0x04;  						/*; make sure ITCM mapped to 0x10000000 only */
//<CJ>                	asm ("DSB  ");                      /*; ensure that store completes before      */
                	                                    /*; executing the next instruction  		*/
//<CJ>                	asm ("ISB  ");                      /*; executing synchronization instruction   */
                	/*Set reset vector in LSRAM tyo zero */
//<CJ>                	*(uint32_t *)0x00000004 = 0;
//<CJ>                	NVIC_SystemReset();		/* RESET the system- program will be loaded by CoreBootStrap */
			break;
            }
        }
    }
    UART_polled_tx_string( &g_uart, g_load_executable_msg);
    read_program_from_flash((uint8_t *)DDR_BASE_ADDRESS, (128 * 1024));
    UART_polled_tx_string( &g_uart, g_run_executable_msg);
    Bootloader_JumpToApplication(0x70000000, 0x70000004);
    /* will never reach here! */
    while(1);
    return 0;
}

/*
 *  Test flash on RTG4
 */
static int test_flash(void)
{
    uint8_t write_buffer[FLASH_SEGMENT_SIZE];
    uint8_t read_buffer[FLASH_SEGMENT_SIZE];
    uint16_t status;
    int flash_address = 0;
    int count = 0;
    spi_flash_status_t result;
    struct device_Info DevInfo;

    spi_flash_init();

    spi_flash_control_hw( SPI_FLASH_RESET, 0, &status );

    result = spi_flash_control_hw( SPI_FLASH_READ_DEVICE_ID,
                                          count * FLASH_SECTOR_SIZE,
                                           &DevInfo );

    result = spi_flash_control_hw( SPI_FLASH_READ_DEVICE_ID,
                                              count * FLASH_SECTOR_SIZE,
                                               &DevInfo );

    /*--------------------------------------------------------------------------
     * First fetch status register. First byte in low 8 bits, second byte in
    * upper 8 bits.
     */
    result = spi_flash_control_hw( SPI_FLASH_GET_STATUS, 0, &status );

    result = spi_flash_control_hw( SPI_FLASH_READ_DEVICE_ID,
                                              count * FLASH_SECTOR_SIZE,
                                               &DevInfo );




    /*--------------------------------------------------------------------------
     * Fetch protection register value for each of the 128 sectors.
     * After power up these should all read as 0xFF
     */
   for( count = 0; count != 128; ++count )
   {
    result = spi_flash_control_hw( SPI_FLASH_GET_PROTECT,
                                      count * FLASH_SECTOR_SIZE,
                                       &read_buffer[count] );
   }

   //device D
   result = spi_flash_control_hw( SPI_FLASH_READ_DEVICE_ID,
                                              count * FLASH_SECTOR_SIZE,
                                               &DevInfo );

   /*--------------------------------------------------------------------------
    * Show sector protection in action by:
    *   - unprotecting the first sector
    *   - erasing the sector
    *   - writing some data to the first 256 bytes
    *   - protecting the first sector
    *   - erasing the first sector
    *   - reading back the first 256 bytes of the first sector
    *   - unprotecting the first sector
    *   - erasing the sector
    *   - reading back the first 256 bytes of the first sector
    *
    * The first read should still show the written data in place as the erase
    * will fail. the second read should show all 0xFFs. Step through the code
    * in debug mode and examine the read buffer after the read operations to
    * see this.
    */
   result = spi_flash_control_hw( SPI_FLASH_SECTOR_UNPROTECT, flash_address, NULL );
   //device D   works
   result = spi_flash_control_hw( SPI_FLASH_READ_DEVICE_ID,
                                              count * FLASH_SECTOR_SIZE,
                                               &DevInfo );
    result = spi_flash_control_hw( SPI_FLASH_4KBLOCK_ERASE, flash_address , NULL );
       //device D-- now working
       result = spi_flash_control_hw( SPI_FLASH_READ_DEVICE_ID,
                                                  count * FLASH_SECTOR_SIZE,
                                                   &DevInfo );
    memset( write_buffer, count, FLASH_SEGMENT_SIZE );
    strcpy( (char *)write_buffer, "Microsemi FLASH test" );
    spi_flash_write( flash_address, write_buffer, FLASH_SEGMENT_SIZE );
       //device D --
       result = spi_flash_control_hw( SPI_FLASH_READ_DEVICE_ID,
                                                  count * FLASH_SECTOR_SIZE,
                                                   &DevInfo );

    result = spi_flash_control_hw( SPI_FLASH_SECTOR_PROTECT, flash_address, NULL );
       //device D
       result = spi_flash_control_hw( SPI_FLASH_READ_DEVICE_ID,
                                                  count * FLASH_SECTOR_SIZE,
                                                   &DevInfo );
    result = spi_flash_control_hw( SPI_FLASH_4KBLOCK_ERASE, flash_address , NULL );
       //device D
       result = spi_flash_control_hw( SPI_FLASH_READ_DEVICE_ID,
                                                  count * FLASH_SECTOR_SIZE,
                                                   &DevInfo );
    result = spi_flash_control_hw( SPI_FLASH_GET_STATUS, 0, &status );
       //device D
       result = spi_flash_control_hw( SPI_FLASH_READ_DEVICE_ID,
                                                  count * FLASH_SECTOR_SIZE,
                                                   &DevInfo );

    spi_flash_read ( flash_address, read_buffer, FLASH_SEGMENT_SIZE);
       //device D
       result = spi_flash_control_hw( SPI_FLASH_READ_DEVICE_ID,
                                                  count * FLASH_SECTOR_SIZE,
                                                   &DevInfo );

    result = spi_flash_control_hw( SPI_FLASH_SECTOR_UNPROTECT, flash_address, NULL );
       //device D
       result = spi_flash_control_hw( SPI_FLASH_READ_DEVICE_ID,
                                                  count * FLASH_SECTOR_SIZE,
                                                   &DevInfo );
    result = spi_flash_control_hw( SPI_FLASH_4KBLOCK_ERASE, flash_address , NULL );
       //device D
       result = spi_flash_control_hw( SPI_FLASH_READ_DEVICE_ID,
                                                  count * FLASH_SECTOR_SIZE,
                                                   &DevInfo );
    result = spi_flash_control_hw( SPI_FLASH_GET_STATUS, 0, &status );
       //device D
       result = spi_flash_control_hw( SPI_FLASH_READ_DEVICE_ID,
                                                  count * FLASH_SECTOR_SIZE,
                                                   &DevInfo );

    result = spi_flash_control_hw( SPI_FLASH_SECTOR_UNPROTECT, flash_address, NULL );
       //device D
       result = spi_flash_control_hw( SPI_FLASH_READ_DEVICE_ID,
                                                  count * FLASH_SECTOR_SIZE,
                                                   &DevInfo );
    result = spi_flash_control_hw( SPI_FLASH_4KBLOCK_ERASE, flash_address , NULL );
       //device D
       result = spi_flash_control_hw( SPI_FLASH_READ_DEVICE_ID,
                                                  count * FLASH_SECTOR_SIZE,
                                                   &DevInfo );

    spi_flash_read ( flash_address, read_buffer, FLASH_SEGMENT_SIZE );
       //device D
       result = spi_flash_control_hw( SPI_FLASH_READ_DEVICE_ID,
                                                  count * FLASH_SECTOR_SIZE,
                                                   &DevInfo );
    /*--------------------------------------------------------------------------
     * Read the protection registers again so you can see that the first sector
     * is unprotected now.
     */
    for( count = 0; count != 128; ++count )
   {
    spi_flash_control_hw( SPI_FLASH_GET_PROTECT, count * FLASH_SECTOR_SIZE,
                             &write_buffer[count] );
   }
       //device D
       result = spi_flash_control_hw( SPI_FLASH_READ_DEVICE_ID,
                                                  count * FLASH_SECTOR_SIZE,
                                                   &DevInfo );
    /*--------------------------------------------------------------------------
     * Write something to all 32768 blocks of 256 bytes in the 8MB FLASH.
     */
   for( count = 0; count != 1000 /*32768*/; ++count )
   {
        /*----------------------------------------------------------------------
         * Vary the fill for each chunk of 256 bytes
         */
        memset( write_buffer, count, FLASH_SEGMENT_SIZE );
        strcpy( (char *)write_buffer, "Microsemi FLASH test" );
        /*----------------------------------------------------------------------
         * at the start of each sector we need to make sure it is unprotected
         * so we can erase blocks within it. The spi_flash_write() function
         * unprotects the sector as well but we need to start erasing before the
         * first write takes place.
         */
        if(0 == (flash_address % FLASH_SECTOR_SIZE))
        {
            result = spi_flash_control_hw( SPI_FLASH_SECTOR_UNPROTECT, flash_address, NULL );
        }
        /*----------------------------------------------------------------------
         * At the start of each 4K block we issue an erase so that we are then
         * free to write anything we want to the block. If we don't do this the
         * write may fail as we can only effectively turn 1s to 0s when we
         * write. For example if we have an erased location with 0xFF in it and
         * we write 0xAA to it first and then later on write 0x55, the resulting
         * value is 0x00...
         */
        if(0 == (flash_address % FLASH_BLOCK_SIZE))
        {
            result = spi_flash_control_hw( SPI_FLASH_4KBLOCK_ERASE, flash_address , NULL );
        }
        /*----------------------------------------------------------------------
         * Write our values to the FLASH, read them back and compare.
         * Placing a breakpoint on the while statement below will allow
         * you break on any failures.
         */
        spi_flash_write( flash_address, write_buffer, FLASH_SEGMENT_SIZE );
        spi_flash_read ( flash_address, read_buffer, FLASH_SEGMENT_SIZE );
        if( memcmp( write_buffer, read_buffer, FLASH_SEGMENT_SIZE ) )
        {
            while(1) // Breakpoint here will trap write faults
            {
                   result = spi_flash_control_hw( SPI_FLASH_READ_DEVICE_ID,
                                                              count * FLASH_SECTOR_SIZE,
                                                               &DevInfo );
                   spi_flash_control_hw( SPI_FLASH_RESET, 0, &status );

            }

        }

        flash_address += FLASH_SEGMENT_SIZE; /* Step to the next 256 byte chunk */
    }
   /*--------------------------------------------------------------------------
    * One last look at the protection registers which should all be 0 now
    */
   for( count = 0; count != 128; ++count )
   {
    spi_flash_control_hw( SPI_FLASH_GET_PROTECT, count * FLASH_SECTOR_SIZE,
                            &write_buffer[count] );
   }

   UART_polled_tx_string( &g_uart, "  Flash test success\n\r" );

   return(0);
}



/*
 *  Write to flash on RTG4
 */
static int write_program_to_flash(uint8_t *write_buf, uint32_t file_size)
{
    uint8_t write_buffer[FLASH_SEGMENT_SIZE];
    uint8_t read_buffer[FLASH_SEGMENT_SIZE];
    uint16_t status;
    int flash_address = 0;
    int count = 0;
    spi_flash_status_t result;
    struct device_Info DevInfo;

    UART_polled_tx_string( &g_uart, "\r\n---------------------- Writing SPI flash from DDR memory ----------------------\r\n" );
    UART_polled_tx_string( &g_uart, "This may take several minutes to complete if writing a large file.\r\n" );

    spi_flash_init();

    spi_flash_control_hw( SPI_FLASH_RESET, 0, &status );

    result = spi_flash_control_hw( SPI_FLASH_READ_DEVICE_ID,
                                          count * FLASH_SECTOR_SIZE,
                                           &DevInfo );

    result = spi_flash_control_hw( SPI_FLASH_READ_DEVICE_ID,
                                              count * FLASH_SECTOR_SIZE,
                                               &DevInfo );

    /*--------------------------------------------------------------------------
     * First fetch status register. First byte in low 8 bits, second byte in
    * upper 8 bits.
     */
    result = spi_flash_control_hw( SPI_FLASH_GET_STATUS, 0, &status );

    result = spi_flash_control_hw( SPI_FLASH_READ_DEVICE_ID,
                                              count * FLASH_SECTOR_SIZE,
                                               &DevInfo );




    /*--------------------------------------------------------------------------
     * Fetch protection register value for each of the 128 sectors.
     * After power up these should all read as 0xFF
     */
   for( count = 0; count != 128; ++count )
   {
    result = spi_flash_control_hw( SPI_FLASH_GET_PROTECT,
                                      count * FLASH_SECTOR_SIZE,
                                       &read_buffer[count] );
   }

   //device D
   result = spi_flash_control_hw( SPI_FLASH_READ_DEVICE_ID,
                                              count * FLASH_SECTOR_SIZE,
                                               &DevInfo );

   /*--------------------------------------------------------------------------
    * Show sector protection in action by:
    *   - unprotecting the first sector
    *   - erasing the sector
    *   - writing some data to the first 256 bytes
    *   - protecting the first sector
    *   - erasing the first sector
    *   - reading back the first 256 bytes of the first sector
    *   - unprotecting the first sector
    *   - erasing the sector
    *   - reading back the first 256 bytes of the first sector
    *
    * The first read should still show the written data in place as the erase
    * will fail. the second read should show all 0xFFs. Step through the code
    * in debug mode and examine the read buffer after the read operations to
    * see this.
    */
   result = spi_flash_control_hw( SPI_FLASH_SECTOR_UNPROTECT, flash_address, NULL );
   //device D   works
   result = spi_flash_control_hw( SPI_FLASH_READ_DEVICE_ID,
                                              count * FLASH_SECTOR_SIZE,
                                               &DevInfo );
    result = spi_flash_control_hw( SPI_FLASH_4KBLOCK_ERASE, flash_address , NULL );
       //device D-- now working
       result = spi_flash_control_hw( SPI_FLASH_READ_DEVICE_ID,
                                                  count * FLASH_SECTOR_SIZE,
                                                   &DevInfo );
    memset( write_buffer, count, FLASH_SEGMENT_SIZE );
    strcpy( (char *)write_buffer, "Microsemi FLASH test" );
    spi_flash_write( flash_address, write_buffer, FLASH_SEGMENT_SIZE );
       //device D --
       result = spi_flash_control_hw( SPI_FLASH_READ_DEVICE_ID,
                                                  count * FLASH_SECTOR_SIZE,
                                                   &DevInfo );

    result = spi_flash_control_hw( SPI_FLASH_SECTOR_PROTECT, flash_address, NULL );
       //device D
       result = spi_flash_control_hw( SPI_FLASH_READ_DEVICE_ID,
                                                  count * FLASH_SECTOR_SIZE,
                                                   &DevInfo );
    result = spi_flash_control_hw( SPI_FLASH_4KBLOCK_ERASE, flash_address , NULL );
       //device D
       result = spi_flash_control_hw( SPI_FLASH_READ_DEVICE_ID,
                                                  count * FLASH_SECTOR_SIZE,
                                                   &DevInfo );
    result = spi_flash_control_hw( SPI_FLASH_GET_STATUS, 0, &status );
       //device D
       result = spi_flash_control_hw( SPI_FLASH_READ_DEVICE_ID,
                                                  count * FLASH_SECTOR_SIZE,
                                                   &DevInfo );

    spi_flash_read ( flash_address, read_buffer, FLASH_SEGMENT_SIZE);
       //device D
       result = spi_flash_control_hw( SPI_FLASH_READ_DEVICE_ID,
                                                  count * FLASH_SECTOR_SIZE,
                                                   &DevInfo );

    result = spi_flash_control_hw( SPI_FLASH_SECTOR_UNPROTECT, flash_address, NULL );
       //device D
       result = spi_flash_control_hw( SPI_FLASH_READ_DEVICE_ID,
                                                  count * FLASH_SECTOR_SIZE,
                                                   &DevInfo );
    result = spi_flash_control_hw( SPI_FLASH_4KBLOCK_ERASE, flash_address , NULL );
       //device D
       result = spi_flash_control_hw( SPI_FLASH_READ_DEVICE_ID,
                                                  count * FLASH_SECTOR_SIZE,
                                                   &DevInfo );
    result = spi_flash_control_hw( SPI_FLASH_GET_STATUS, 0, &status );
       //device D
       result = spi_flash_control_hw( SPI_FLASH_READ_DEVICE_ID,
                                                  count * FLASH_SECTOR_SIZE,
                                                   &DevInfo );

    result = spi_flash_control_hw( SPI_FLASH_SECTOR_UNPROTECT, flash_address, NULL );
       //device D
       result = spi_flash_control_hw( SPI_FLASH_READ_DEVICE_ID,
                                                  count * FLASH_SECTOR_SIZE,
                                                   &DevInfo );
    result = spi_flash_control_hw( SPI_FLASH_4KBLOCK_ERASE, flash_address , NULL );
       //device D
       result = spi_flash_control_hw( SPI_FLASH_READ_DEVICE_ID,
                                                  count * FLASH_SECTOR_SIZE,
                                                   &DevInfo );

    spi_flash_read ( flash_address, read_buffer, FLASH_SEGMENT_SIZE );
       //device D
       result = spi_flash_control_hw( SPI_FLASH_READ_DEVICE_ID,
                                                  count * FLASH_SECTOR_SIZE,
                                                   &DevInfo );
    /*--------------------------------------------------------------------------
     * Read the protection registers again so you can see that the first sector
     * is unprotected now.
     */
    for( count = 0; count != 128; ++count )
    {
        spi_flash_control_hw( SPI_FLASH_GET_PROTECT, count * FLASH_SECTOR_SIZE,
                             &write_buffer[count] );
    }
       //device D
       result = spi_flash_control_hw( SPI_FLASH_READ_DEVICE_ID,
                                                  count * FLASH_SECTOR_SIZE,
                                                   &DevInfo );
    /*--------------------------------------------------------------------------
     * Write something to all 32768 blocks of 256 bytes in the 8MB FLASH.
     */
    uint32_t nb_blocks_to_write;
    nb_blocks_to_write = (file_size / FLASH_SEGMENT_SIZE);
    if ((file_size % FLASH_SEGMENT_SIZE) > 0)
    {
        ++nb_blocks_to_write;
    }

    for( count = 0; count != nb_blocks_to_write; ++count )
    {
        /*----------------------------------------------------------------------
         * Vary the fill for each chunk of 256 bytes
         */
        memset( write_buffer, count, FLASH_SEGMENT_SIZE );
        strcpy( (char *)write_buffer, "Microsemi FLASH test" );
        /*----------------------------------------------------------------------
         * at the start of each sector we need to make sure it is unprotected
         * so we can erase blocks within it. The spi_flash_write() function
         * unprotects the sector as well but we need to start erasing before the
         * first write takes place.
         */
        if(0 == (flash_address % FLASH_SECTOR_SIZE))
        {
            result = spi_flash_control_hw( SPI_FLASH_SECTOR_UNPROTECT, flash_address, NULL );
        }
        /*----------------------------------------------------------------------
         * At the start of each 4K block we issue an erase so that we are then
         * free to write anything we want to the block. If we don't do this the
         * write may fail as we can only effectively turn 1s to 0s when we
         * write. For example if we have an erased location with 0xFF in it and
         * we write 0xAA to it first and then later on write 0x55, the resulting
         * value is 0x00...
         */
        if(0 == (flash_address % FLASH_BLOCK_SIZE))
        {
            result = spi_flash_control_hw( SPI_FLASH_4KBLOCK_ERASE, flash_address , NULL );
        }
        /*----------------------------------------------------------------------
         * Write our values to the FLASH, read them back and compare.
         * Placing a breakpoint on the while statement below will allow
         * you break on any failures.
         */
        spi_flash_write( flash_address, write_buf, FLASH_SEGMENT_SIZE );

        spi_flash_read ( flash_address, read_buffer, FLASH_SEGMENT_SIZE );
        if( memcmp( write_buf, read_buffer, FLASH_SEGMENT_SIZE ) )
        {
            while(1) // Breakpoint here will trap write faults
            {

            }

        }
        write_buf += FLASH_SEGMENT_SIZE;
        flash_address += FLASH_SEGMENT_SIZE; /* Step to the next 256 byte chunk */
        show_progress();
    }

    /*--------------------------------------------------------------------------
     * Record the size written in the first SPI flash segment.
     */
    {
        flash_content_t flash_content;

        flash_content.validity_key = SPI_FLASH_VALID_CONTENT_KEY;
        flash_content.spi_content_byte_size = file_size;

        flash_address = LAST_BLOCK_ADDR;

        /*----------------------------------------------------------------------
         * at the start of each sector we need to make sure it is unprotected
         * so we can erase blocks within it. The spi_flash_write() function
         * unprotects the sector as well but we need to start erasing before the
         * first write takes place.
         */
        if(0 == (flash_address % FLASH_SECTOR_SIZE))
        {
            result = spi_flash_control_hw( SPI_FLASH_SECTOR_UNPROTECT, flash_address, NULL );
        }
        /*----------------------------------------------------------------------
         * At the start of each 4K block we issue an erase so that we are then
         * free to write anything we want to the block. If we don't do this the
         * write may fail as we can only effectively turn 1s to 0s when we
         * write. For example if we have an erased location with 0xFF in it and
         * we write 0xAA to it first and then later on write 0x55, the resulting
         * value is 0x00...
         */
        if(0 == (flash_address % FLASH_BLOCK_SIZE))
        {
            result = spi_flash_control_hw( SPI_FLASH_4KBLOCK_ERASE, flash_address , NULL );
        }
        /*----------------------------------------------------------------------
         * Write our values to the FLASH, read them back and compare.
         * Placing a breakpoint on the while statement below will allow
         * you break on any failures.
         */
        spi_flash_write( flash_address, (uint8_t *)(&flash_content), FLASH_SEGMENT_SIZE );

        spi_flash_read ( flash_address, read_buffer, FLASH_SEGMENT_SIZE );
        if( memcmp( (uint8_t *)&flash_content, read_buffer, FLASH_SEGMENT_SIZE ) )
        {
            while(1) // Breakpoint here will trap write faults
            {

            }

        }
        write_buf += FLASH_SEGMENT_SIZE;
        flash_address += FLASH_SEGMENT_SIZE; /* Step to the next 256 byte chunk */
        show_progress();
    }

    /*--------------------------------------------------------------------------
     * One last look at the protection registers which should all be 0 now
     */
    for( count = 0; count != 128; ++count )
    {
    	spi_flash_control_hw( SPI_FLASH_GET_PROTECT, count * FLASH_SECTOR_SIZE,
                            &write_buffer[count] );
    }

    UART_polled_tx_string( &g_uart, "  Flash write success\n\r" );

    return(0);
}


/**
 *  Read from flash
 */
static int read_program_from_flash(uint8_t *read_buf, uint32_t read_byte_length)
{
    uint16_t status;
    int flash_address = 0;
    int count = 0;
    uint32_t nb_segments_to_read;
    spi_flash_status_t result;
    struct device_Info DevInfo;
    flash_content_t flash_content;

    UART_polled_tx_string( &g_uart, "\r\n------------------- Reading from SPI flash into DDR memory --------------------\r\n" );
    UART_polled_tx_string( &g_uart, "This will take several minutes to complete in order to read the full SPI flash \r\ncontent.\r\n" );

    spi_flash_init();

    spi_flash_control_hw( SPI_FLASH_RESET, 0, &status );

    result = spi_flash_control_hw( SPI_FLASH_READ_DEVICE_ID,
                                          count * FLASH_SECTOR_SIZE,
                                           &DevInfo );

    result = spi_flash_control_hw( SPI_FLASH_READ_DEVICE_ID,
                                              count * FLASH_SECTOR_SIZE,
                                               &DevInfo );

    /*--------------------------------------------------------------------------
     * First fetch status register. First byte in low 8 bits, second byte in
    * upper 8 bits.
     */
    result = spi_flash_control_hw( SPI_FLASH_GET_STATUS, 0, &status );

    result = spi_flash_control_hw( SPI_FLASH_READ_DEVICE_ID,
                                              count * FLASH_SECTOR_SIZE,
                                               &DevInfo );


    /*--------------------------------------------------------------------------
     * Retrieve the size of the data previously written to SPI flash.
     */
    spi_flash_read ( LAST_BLOCK_ADDR, (uint8_t *)&flash_content, FLASH_SEGMENT_SIZE );

    if(SPI_FLASH_VALID_CONTENT_KEY == flash_content.validity_key)
    {
        read_byte_length = flash_content.spi_content_byte_size;
    }
    else
    {
        read_byte_length = 0;
    }

    /*--------------------------------------------------------------------------
     * Read from flash 256 bytes increments (FLASH_SEGMENT_SIZE).
     */
    nb_segments_to_read = read_byte_length / FLASH_SEGMENT_SIZE;
    if((read_byte_length % FLASH_SEGMENT_SIZE) > 0)
    {
    	++nb_segments_to_read;
    }

    for( count = 0; count != nb_segments_to_read; ++count )
    {
        /*----------------------------------------------------------------------
         * Write our values to the FLASH, read them back and compare.
         * Placing a breakpoint on the while statement below will allow
         * you break on any failures.
         */

        spi_flash_read ( flash_address, read_buf, FLASH_SEGMENT_SIZE );
        read_buf += FLASH_SEGMENT_SIZE;

        flash_address += FLASH_SEGMENT_SIZE; /* Step to the next 256 byte chunk */

        show_progress();
    }

    UART_polled_tx_string( &g_uart, "  Flash read success\n\r" );

    return(0);
}

/*
 * Simple sanity check
 */
static void mem_test(uint8_t *address)
{
    volatile uint8_t value=2;
    volatile uint32_t value32=3;
    *address = 1;
    value = *address;
    value32 = (uint32_t)*address;
    if((value32 == value) &&(value == 1))
    	UART_polled_tx_string( &g_uart, "  Read/Write success\n\r" );
    else
    	UART_polled_tx_string( &g_uart, "  Read/Write fail\n\r" );
}


/*
 * Put image received via ymodem into memory
 */
static uint32_t rx_app_file(uint8_t *dest_address)
{
	uint32_t received;
    uint8_t *g_bin_base = (uint8_t *)dest_address;
    uint32_t g_rx_size = 1024 * 1024 * 8;

    UART_polled_tx_string( &g_uart, "\r\n------------------------ Starting YModem file transfer ------------------------\r\n" );
    UART_polled_tx_string( &g_uart, "Please select file and initiate transfer on host computer.\r\n" );

    received = ymodem_receive(g_bin_base, g_rx_size);

    return received;
}

/*
 *
 */
static void show_progress(void)
{
    static uint32_t progress_count = 0;
    static uint32_t dot_count = 0;

    if (progress_count > 10)
    {
        UART_polled_tx_string( &g_uart, (const uint8_t*)"." );
        progress_count = 0;
        ++dot_count;

    }
    ++progress_count;

    if (dot_count > 78)
    {
        UART_polled_tx_string( &g_uart, (const uint8_t*)"\r\n" );
        dot_count = 0;
    }
}

/*------------------------------------------------------------------------------
 * Count the number of elapsed milliseconds (SysTick_Handler is called every
 * 10mS so the resolution will be 10ms). Rolls over every 49 days or so...
 *
 * Should be safe to read g_10ms_count from elsewhere.
 */
void SysTick_Handler( void )
{
    uint32_t gpio_pattern;
    static uint8_t count;
    /*
     * Toggle GPIO output pattern by doing an exclusive OR of all
     * pattern bits with ones.
     */
    if(count++>=50)
    {
        gpio_pattern = GPIO_get_outputs( &g_gpio );
        gpio_pattern ^= 0x00000002;
        GPIO_set_outputs( &g_gpio, gpio_pattern );
        count=0;
    }

    g_10ms_count += 10;

     /*
      * For neatness, if we roll over, reset cleanly back to 0 so the count
      * always goes up in proper 10s.
      */
    if(g_10ms_count < 10)
        g_10ms_count = 0;
}

/*------------------------------------------------------------------------------
 * Call this function if you want to switch to another program
 * de-init any loaded drivers before calling this function
 */
//volatile uint32_t cj_debug;
static void Bootloader_JumpToApplication(uint32_t stack_location, uint32_t reset_vector)
{
	/* Restore PLIC to known state: */
	__disable_irq();
	PLIC_init();

	/* Disable all interrupts: */
	write_csr(mie, 0);

	/* Start executing from the top of DDR memory: */
    __asm volatile("lui ra,0x80000");
    __asm volatile("ret");
    
    /*User application execution should now start and never return here.... */
}



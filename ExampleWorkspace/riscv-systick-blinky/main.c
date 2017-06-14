/*******************************************************************************
 * (c) Copyright 2016-2017 Microsemi SoC Products Group. All rights reserved.
 * 
 * This SoftConsole example project demonstrates how to use configure and use the
 * CoreRISCV_AXI4 system timer.
 *
 * Please refer README.TXT in the root folder of this project for more details.
 */
#include "riscv_hal.h"
#include "hw_platform.h"
#include "core_uart_apb.h"
#include "core_gpio.h"

const char * g_hello_msg =
"\r\nSystem timer Blinky Example. \n\r\
 Observe the LEDs blinking on the board. The LED patterns changes \
 every time a system timer interrupt occurs\r\n";

/*-----------------------------------------------------------------------------
 * UART instance data.
 */
UART_instance_t g_uart;
  
/*-----------------------------------------------------------------------------
 * GPIO instance data.
 */

gpio_instance_t g_gpio_in;
gpio_instance_t g_gpio_out;

/*-----------------------------------------------------------------------------
 * Global state counter.
 */
uint32_t g_state = 1;

/*-----------------------------------------------------------------------------
 * System Tick interrupt handler
 */
void SysTick_Handler(void) {
    uint32_t stable;
    uint32_t gpout;

    stable = GPIO_get_inputs(&g_gpio_in);
    gpout = ~stable & 0x000000F0;

    g_state = g_state << 1;
    if (g_state > 4) {
        g_state = 0x01;
    }
    gpout = gpout | g_state;

    GPIO_set_outputs(&g_gpio_out, gpout);
}

/*-----------------------------------------------------------------------------
 * main
 */
int main(int argc, char **argv) {
    uint8_t rx_char;
    uint8_t rx_count;

    PLIC_init();

    GPIO_init(&g_gpio_in, COREGPIO_IN_BASE_ADDR, GPIO_APB_32_BITS_BUS);
    GPIO_init(&g_gpio_out, COREGPIO_OUT_BASE_ADDR, GPIO_APB_32_BITS_BUS);

    UART_init(&g_uart,
              COREUARTAPB0_BASE_ADDR,
			  BAUD_VALUE_115200,
              (DATA_8_BITS | NO_PARITY));

    UART_polled_tx_string(&g_uart, (const uint8_t *)g_hello_msg);

    SysTick_Config(SYS_CLK_FREQ / 2);

    /*
     * Loop. Echo back characters received on UART.
     */
    do {
        rx_count = UART_get_rx(&g_uart, &rx_char, 1);
        if (rx_count > 0) {
            UART_send(&g_uart, &rx_char, 1);
        }
    } while (1);
  
  
    return 0;
}


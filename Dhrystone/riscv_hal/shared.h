// See LICENSE for license details.

#ifndef SIFIVE_SHARED_H
#define SIFIVE_SHARED_H

#include "riscv_hal.h"
#include "../drivers/CoreUARTapb/core_uart_apb.h"
#include "../drivers/CoreGPIO/core_gpio.h"
#include "../drivers/CoreTIMER/core_timer.h"
#include "riscv_plic.h"

#define PLIC_BASE_ADDR         0x40000000UL
#define COREUARTAPB0_BASE_ADDR 0x70001000UL
#define COREGPIO_IN_BASE_ADDR     0x70002000UL
#define COREGPIO_OUT_BASE_ADDR     0x70005000UL

#define CORETIMER0_BASE_ADDR   0x70003000UL


#define CORETIMER1_BASE_ADDR   0x70004000UL

/****************************************************************************
 * External Interrupts. Note that these are off-by-one from their pin of 
 * InterruptVector[30:0], because PLIC's DEVICE_0 is unused. 
 *****************************************************************************/
// Buttons & Switches
#define INT_DEVICE_BUTTON_0 1
#define INT_DEVICE_BUTTON_1 2
#define INT_DEVICE_BUTTON_2 3
#define INT_DEVICE_BUTTON_3 4
#define INT_DEVICE_SWITCHES_0 5
#define INT_DEVICE_SWITCHES_1 6
#define INT_DEVICE_SWITCHES_2 7
#define INT_DEVICE_SWITCHES_3 8
//UART
#define INT_DEVICE_UTXRDY      25
#define INT_DEVICE_URXRDY      26
#define INT_DEVICE_UPARITY     27
#define INT_DEVICE_UOVERFLOW   28
#define INT_DEVICE_UFRAMING    29
//CoreTimer
#define INT_DEVICE_TIMER0      30
#define INT_DEVICE_TIMER1      31

// Setting these correctly makes the initialization scripts
// run faster.
#define PLIC_NUM_SOURCES 31
#define PLIC_NUM_PRIORITIES 0

extern UART_instance_t g_uart;

void write_hex(int fd, uint32_t hex);

/****************************************************************************
 * Baud value to achieve a 115200 baud rate with a 100MHz system clock.
 * This value is calculated using the following equation:
 *      BAUD_VALUE = (CLOCK / (16 * BAUD_RATE)) - 1
 *****************************************************************************/
//#define BAUD_VALUE_115200   53

/****************************************************************************
 * Baud value to achieve a 115200 baud rate with a 83MHz system clock.
 * This value is calculated using the following equation:
 *      BAUD_VALUE = (CLOCK / (16 * BAUD_RATE)) - 1
 *****************************************************************************/
#define BAUD_VALUE_115200   44

#endif

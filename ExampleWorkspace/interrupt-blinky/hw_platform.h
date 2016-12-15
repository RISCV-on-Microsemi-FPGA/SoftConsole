/*******************************************************************************
 * (c) Copyright 2016 Microsemi Corporation.  All rights reserved.
 *
 * Platform definitions
 * Version based on requirements of Cortex-M1 CMSIS HAL v2.0.0
 *
 * SVN $Revision: $
 * SVN $Date: $
 */
 /*=========================================================================*//**
  @mainpage Sample file detailing how hw_platform.h should be constructed for Cortex-M1 CMSIS HAL v2.0

    @section intro_sec Introduction
    Cortex-M1 CMSIS HAL expects a file named hw_platform.h to be located in the project
    root directory.
    Currently (Libero 11.7) this file must be hand crafted when using the Cortex-M1
    with RTG4. The intention is hw_platform.h will be produced by Libero in the future.
    This file will be placed in a subdirectory of the firmware directory in the Libero
    project file. The firmware subdirectory will be given the name of the Core and
    its instance number.
    e.g. ./firmware/cortexm1cfg_0/hw_platform.h

    In the interim, a hw_platform.h file should be constructed based on the file
    sample_hw_platform.h which is located in the CMSIS directory
    ./HAL/CortexM1/

    @section driver_configuration Project configuration Instructions
    1. Copy sample_hw_platform.h to the project root directory from ./HAL/CortexM1
    2. Rename sample_hw_platform.h to hw_platform.h
    3. Change SYS_M1_CLK_FREQ define to frequency of Cortex-M1 clock
    4. Add memory core addresses- These are generally just used for creating linker script
    5. Add sizes of all memory Cores- These are generally just used for creating linker script
    6. Add all other core BASE addresses
    7. Add peripheral Core Interrupt to Cortex-M1 interrupt mappings
    8. Define MSCC_STDIO_UART_BASE_ADDR if you want a CoreUARTapb mapped to STDIO
*//*=========================================================================*/

#ifndef HW_PLATFORM_H
#define HW_PLATFORM_H

/***************************************************************************//**
 * Soft-processor clock definition
 * This is the only clock brought over from the Cortex-M1 Libero design.
 * The current design requirement is all the associated Cores in the Cortex-M1
 * subsystem are running at this frequency.
 */
#define SYS_CLK_FREQ                    83000000UL

/***************************************************************************//**
 * Memory Cores addresses-
 * RAM and NVM base addresses can be referenced here when editing to projects
 * linker script.
 * Format of define is:
 * <corename>_<instance>_ADDR
 */
/* e.g.
#define COREAHBSRAM_0_BASE_ADDR         0x00000000UL
#define COREAHBNVM_0_BASE_ADDR          0x10000000UL
*/

/***************************************************************************//**
 * Memory Cores sizes-
 * RAM and NVM sizes can be referenced here when editing projects
 * linker scripts.
 * Format of define is:
 * <corename>_<instance>_SIZE
 */
/* e.g.
#define COREAHBSRAM_0_SIZE              0x00002800UL
#define COREAHBNVM_0_SIZE               0x00002800UL
*/

/***************************************************************************//**
 * Non-memory Peripheral base addresses
 * Format of define is:
 * <corename>_<instance>_BASE_ADDR
 */
#define COREUARTAPB0_BASE_ADDR          0x70001000UL
#define COREGPIO_BASE_ADDR              0x70002000UL
#define COREGPIO_IN_BASE_ADDR           0x70002000UL
#define CORETIMER0_BASE_ADDR            0x70003000UL
#define CORETIMER1_BASE_ADDR            0x70004000UL
#define COREGPIO_OUT_BASE_ADDR          0x70005000UL
#define FLASH_CORE_SPI_BASE             0x70006000UL

/***************************************************************************//**
 * Peripheral Interrupts are mapped to the corresponding Cortex-M1 interrupt
 * from the Libero design.
 * There can be upto 32 external interrupts (0-31) on the Cortex-M1
 * The Cortex-M1 external interrupts are defined in the Cortex-M1 CMSIS HAL
 * in the file cortexm1_cfg.h
 * These are of the form
 * #define ExternalIrq0_IRQn = 0
 * The interrupt is enabled in the user code as follows:
 * First the Core is initialized
 * UART_init( &g_uart, COREUARTAPB1_BASE_ADDR, BAUD_VALUE_57600, (DATA_8_BITS | NO_PARITY) );
 * The Interrupt is then enabled.
 * NVIC_EnableIRQ(UART1_TXRDY_IRQ_NB);
 *
 * Format of define is:
 * <corename>_<instance>_<core interrupt name>
 */

#define TIMER0_IRQn                     External_30_IRQn
#define TIMER1_IRQn                     External_31_IRQn

/****************************************************************************
 * Baud value to achieve a 115200 baud rate with a 83MHz system clock.
 * This value is calculated using the following equation:
 *      BAUD_VALUE = (CLOCK / (16 * BAUD_RATE)) - 1
 *****************************************************************************/
#define BAUD_VALUE_115200   44

/***************************************************************************//**
 * User edit section- Edit sections below if required
 */
#ifdef MSCC_STDIO_THRU_CORE_UART_APB
/*
 * A base address mapping for the STDIO printf/scanf mapping to CortUARTapb
 * must be provided if it is being used
 *
 * e.g. #define MSCC_STDIO_UART_BASE_ADDR COREUARTAPB1_BASE_ADDR
 */
#define MSCC_STDIO_UART_BASE_ADDR COREUARTAPB0_BASE_ADDR
#ifndef MSCC_STDIO_UART_BASE_ADDR
#error MSCC_STDIO_UART_BASE_ADDR not defined- e.g. #define MSCC_STDIO_UART_BASE_ADDR COREUARTAPB1_BASE_ADDR
#endif
#endif  /* end of MSCC_STDIO_THRU_CORE_UART_APB */
/*******************************************************************************
 * End of user edit section
 */
#endif /* HW_PLATFORM_H */



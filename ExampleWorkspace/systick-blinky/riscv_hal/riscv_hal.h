/*******************************************************************************
 * (c) Copyright 2016 Microsemi SoC Products Group. All rights reserved.
 * 
 * Hardware Abtraction Layer for RISC-V. This is mainly targeted at RV32IM but
 * should be usable with other variants.
 * 
 */

#ifndef RISCV_HAL_H
#define RISCV_HAL_H

#include "riscv_CoreplexE31.h"

/*------------------------------------------------------------------------------
 * System core clock frequency.
 */
extern uint32_t SystemCoreClock;

/*------------------------------------------------------------------------------
 * Interrupt enable/disable.
 */
void __disable_irq(void);
void __enable_irq(void);

/*------------------------------------------------------------------------------
 *  System tick handler. This is generated from the RISC-V machine timer.
 */
void SysTick_Handler(void);

/*------------------------------------------------------------------------------
 * System tick configuration.
 * Configures the machine timer to generate a system tick interrupt at regular
 * intervals.
 * Takes the number of system clock ticks between interrupts.
 * 
 * Returns 0 if successful.
 * Returns 1 if the interrupt interval cannot be achieved.
 */
uint32_t SysTick_Config(uint32_t ticks);

#endif  /* RISCV_HAL_H */

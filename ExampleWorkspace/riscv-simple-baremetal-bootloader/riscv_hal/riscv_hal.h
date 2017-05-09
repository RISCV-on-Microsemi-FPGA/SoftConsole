/*******************************************************************************
 * (c) Copyright 2016-2017 Microsemi SoC Products Group. All rights reserved.
 * 
 * @file riscv_hal.h
 * @author Microsemi SoC Products Group
 * @brief Hardware Abstraction Layer functions for RISC-V soft processor
 *        CoreRISCV_AXI4. This is mainly targeted at RV32IM but should be usable
 *        with other variants.
 *
 * SVN $Revision: 9014 $
 * SVN $Date: 2017-04-19 06:23:23 +0100 (Wed, 19 Apr 2017) $
 */

#ifndef RISCV_HAL_H
#define RISCV_HAL_H

#include "riscv_CoreplexE31.h"

#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __cplusplus
}
#endif

#endif  /* RISCV_HAL_H */


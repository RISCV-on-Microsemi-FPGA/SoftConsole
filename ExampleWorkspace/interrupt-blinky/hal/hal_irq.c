/***************************************************************************//**
 * (c) Copyright 2016 Microsemi SoC Products Group. All rights reserved.
 * 
 * Legacy interrupt control functions for the Microsemi driver library hardware
 * abstraction layer.
 */
#include "hal.h"
#include "riscv_hal.h"

/*------------------------------------------------------------------------------
 * 
 */
void HAL_enable_interrupts(void) {
    __enable_irq();
}

/*------------------------------------------------------------------------------
 * 
 */
psr_t HAL_disable_interrupts(void) {
    psr_t psr;
    psr = read_csr(mstatus);
    __disable_irq();
    return(psr);
}

/*------------------------------------------------------------------------------
 * 
 */
void HAL_restore_interrupts(psr_t saved_psr) {
    write_csr(mstatus, saved_psr);
}

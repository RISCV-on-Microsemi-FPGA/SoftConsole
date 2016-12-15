/*******************************************************************************
 * (c) Copyright 2016 Microsemi SoC Products Group. All rights reserved.
 * 
 * Hardware Abstraction Layer for RISC-V. This is mainly targeted at RV32IM but
 * should be usable with other variants.
 * 
 */
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

#include "riscv_hal.h"
#include "hw_platform.h"

/*------------------------------------------------------------------------------
 * defines
 */
#define MTIMECMP_BASE_ADDR     0x44004000UL
#define MTIME_ADDR             0x4400BFF8UL

#define RTC_PRESCALER 100

#define SUCCESS 0
#define ERROR   1

/*------------------------------------------------------------------------------
 * 
 */
void Invalid_IRQHandler(void);
void External_1_IRQHandler(void);
void External_2_IRQHandler(void);
void External_3_IRQHandler(void);
void External_4_IRQHandler(void);
void External_5_IRQHandler(void);
void External_6_IRQHandler(void);
void External_7_IRQHandler(void);
void External_8_IRQHandler(void);
void External_9_IRQHandler(void);
void External_10_IRQHandler(void);
void External_11_IRQHandler(void);
void External_12_IRQHandler(void);
void External_13_IRQHandler(void);
void External_14_IRQHandler(void);
void External_15_IRQHandler(void);
void External_16_IRQHandler(void);
void External_17_IRQHandler(void);
void External_18_IRQHandler(void);
void External_19_IRQHandler(void);
void External_20_IRQHandler(void);
void External_21_IRQHandler(void);
void External_22_IRQHandler(void);
void External_23_IRQHandler(void);
void External_24_IRQHandler(void);
void External_25_IRQHandler(void);
void External_26_IRQHandler(void);
void External_27_IRQHandler(void);
void External_28_IRQHandler(void);
void External_29_IRQHandler(void);
void External_30_IRQHandler(void);
void External_31_IRQHandler(void);

/*------------------------------------------------------------------------------
 * 
 */
extern void handle_m_ext_interrupt();
extern void handle_m_timer_interrupt();

/*------------------------------------------------------------------------------
 * System core clock frequency
 */
uint32_t SystemCoreClock = SYS_CLK_FREQ;

/*------------------------------------------------------------------------------
 * Increment value for the mtimecmp register in order to achieve a system tick
 * interrupt as specified through the SysTick_Config() function.
 */
static uint64_t g_systick_increment = 0;

/*------------------------------------------------------------------------------
 * Disable all interrupts.
 */
void __disable_irq(void) {
    clear_csr(mstatus, MSTATUS_MIE);
}

/*------------------------------------------------------------------------------
 * Enabler all interrupts.
 */
void __enable_irq(void) {
    set_csr(mstatus, MSTATUS_MIE);
}

/*------------------------------------------------------------------------------
 * Configure the machine timer to generate an interrupt.
 */
uint32_t SysTick_Config(uint32_t ticks) {
    
    uint32_t ret_val = ERROR;
    
    g_systick_increment = ticks / RTC_PRESCALER;

    if (g_systick_increment > 0) {
        uint32_t mhart_id = read_csr(mhartid);
        PRCI->MTIMECMP[mhart_id] = PRCI->MTIME + g_systick_increment;
        set_csr(mie, MIP_MTIP);
        __enable_irq();
        ret_val = SUCCESS;
    }
    
    return ret_val;
}

/*------------------------------------------------------------------------------
 * RISC-V interrupt handler for machine timer interrupts.
 */
void handle_m_timer_interrupt(){
    uint32_t mhart_id = read_csr(mhartid);
    clear_csr(mie, MIP_MTIP);
    PRCI->MTIMECMP[mhart_id] = PRCI->MTIME + g_systick_increment;
    SysTick_Handler();
    set_csr(mie, MIP_MTIP);
}

/*------------------------------------------------------------------------------
 * RISC-V interrupt handler for external interrupts.
 */
void (*ext_irq_handler_table[32])(void) =
{
    Invalid_IRQHandler,
    External_1_IRQHandler,
    External_2_IRQHandler,
    External_3_IRQHandler,
    External_4_IRQHandler,
    External_5_IRQHandler,
    External_6_IRQHandler,
    External_7_IRQHandler,
    External_8_IRQHandler,
    External_9_IRQHandler,
    External_10_IRQHandler,
    External_11_IRQHandler,
    External_12_IRQHandler,
    External_13_IRQHandler,
    External_14_IRQHandler,
    External_15_IRQHandler,
    External_16_IRQHandler,
    External_17_IRQHandler,
    External_18_IRQHandler,
    External_19_IRQHandler,
    External_20_IRQHandler,
    External_21_IRQHandler,
    External_22_IRQHandler,
    External_23_IRQHandler,
    External_24_IRQHandler,
    External_25_IRQHandler,
    External_26_IRQHandler,
    External_27_IRQHandler,
    External_28_IRQHandler,
    External_29_IRQHandler,
    External_30_IRQHandler,
    External_31_IRQHandler
};


/*------------------------------------------------------------------------------
 * 
 */
void handle_m_ext_interrupt(){
    uint32_t int_num  = PLIC_ClaimIRQ();
    ext_irq_handler_table[int_num]();
    PLIC_CompleteIRQ(int_num);
}

/*------------------------------------------------------------------------------
 * Trap/Interrupt handler
 */
uintptr_t handle_trap(uintptr_t mcause, uintptr_t epc)
{
    if (0) {
    // External Machine-Level Interrupt from PLIC
    } else if ((mcause & MCAUSE_INT) && ((mcause & MCAUSE_CAUSE)  == IRQ_M_EXT)) {
        handle_m_ext_interrupt();
    } else if ((mcause & MCAUSE_INT) && ((mcause & MCAUSE_CAUSE)  == IRQ_M_TIMER)) {
        handle_m_timer_interrupt();
    }    
    else {
        write(1, "trap\n", 5);
        _exit(1 + mcause);
    }
    return epc;
}


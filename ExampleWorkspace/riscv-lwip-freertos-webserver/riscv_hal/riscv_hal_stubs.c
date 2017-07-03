/*******************************************************************************
 * (c) Copyright 2016-2017 Microsemi SoC Products Group. All rights reserved.
 * 
 * @file riscv_hal_stubs.c
 * @author Microsemi SoC Products Group
 * @brief RISC-V soft processor CoreRISCV_AXI4 Function stubs.
 * The functions below will only be linked with the application code if the user
 * does not provide an implementation for these functions. These functions are
 * defined with weak linking so that they can be overridden by a function with
 * same prototype in the user's application code.
 *
 * SVN $Revision: 9014 $
 * SVN $Date: 2017-04-19 10:53:23 +0530 (Wed, 19 Apr 2017) $
 */
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

__attribute__((weak)) void handle_m_ext_interrupt()
{
    _exit(10);
}

__attribute__((weak)) void Software_IRQHandler(void)
{
    _exit(10);
}

__attribute__((weak)) void SysTick_Handler(void)
{
    ;
}

__attribute__((weak))  void Invalid_IRQHandler(void)
{
    ;
}

__attribute__((weak))  void External_1_IRQHandler(void)
{
    ;
}

__attribute__((weak))  void External_2_IRQHandler(void)
{
    ;
}

__attribute__((weak))  void External_3_IRQHandler(void)
{
    ;
}

__attribute__((weak))  void External_4_IRQHandler(void)
{
    ;
}

__attribute__((weak))  void External_5_IRQHandler(void)
{
    ;
}

__attribute__((weak))  void External_6_IRQHandler(void)
{
    ;
}

__attribute__((weak))  void External_7_IRQHandler(void)
{
    ;
}

__attribute__((weak))  void External_8_IRQHandler(void)
{
    ;
}

__attribute__((weak))  void External_9_IRQHandler(void)
{
    ;
}

__attribute__((weak))  void External_10_IRQHandler(void)
{
    ;
}

__attribute__((weak))  void External_11_IRQHandler(void)
{
    ;
}

__attribute__((weak))  void External_12_IRQHandler(void)
{
    ;
}

__attribute__((weak))  void External_13_IRQHandler(void)
{
    ;
}

__attribute__((weak))  void External_14_IRQHandler(void)
{
    ;
}

__attribute__((weak))  void External_15_IRQHandler(void)
{
    ;
}

__attribute__((weak))  void External_16_IRQHandler(void)
{
    ;
}

__attribute__((weak))  void External_17_IRQHandler(void)
{
    ;
}

__attribute__((weak))  void External_18_IRQHandler(void)
{
    ;
}

__attribute__((weak))  void External_19_IRQHandler(void)
{
    ;
}

__attribute__((weak))  void External_20_IRQHandler(void)
{
    ;
}

__attribute__((weak))  void External_21_IRQHandler(void)
{
    ;
}

__attribute__((weak))  void External_22_IRQHandler(void)
{
    ;
}

__attribute__((weak))  void External_23_IRQHandler(void)
{
    ;
}

__attribute__((weak))  void External_24_IRQHandler(void)
{
    ;
}

__attribute__((weak))  void External_25_IRQHandler(void)
{
    ;
}

__attribute__((weak))  void External_26_IRQHandler(void)
{
    ;
}

__attribute__((weak))  void External_27_IRQHandler(void)
{
    ;
}

__attribute__((weak))  void External_28_IRQHandler(void)
{
    ;
}

__attribute__((weak))  void External_29_IRQHandler(void)
{
    ;
}

__attribute__((weak))  void External_30_IRQHandler(void)
{
    ;
}

__attribute__((weak))  void External_31_IRQHandler(void)
{
    ;
}

#ifdef __cplusplus
}
#endif


#ifndef RISCV_COREPLEXE31_H
#define RISCV_COREPLEXE31_H

#include <stdint.h>

#include "encoding.h"

#define PLIC_NUM_SOURCES 31
#define PLIC_NUM_PRIORITIES 0

/*==============================================================================
 * Interrupt numbers:
 */
typedef enum
{
    NoInterrupt_IRQn = 0,
    External_1_IRQn  = 1,
    External_2_IRQn  = 2,
    External_3_IRQn  = 3, 
    External_4_IRQn  = 4,
    External_5_IRQn  = 5,
    External_6_IRQn  = 6,
    External_7_IRQn  = 7,
    External_8_IRQn  = 8,
    External_9_IRQn  = 9,
    External_10_IRQn = 10,
    External_11_IRQn = 11,
    External_12_IRQn = 12,
    External_13_IRQn = 13,
    External_14_IRQn = 14,
    External_15_IRQn = 15,
    External_16_IRQn = 16,
    External_17_IRQn = 17,
    External_18_IRQn = 18,
    External_19_IRQn = 19,
    External_20_IRQn = 20,
    External_21_IRQn = 21,
    External_22_IRQn = 22,
    External_23_IRQn = 23,
    External_24_IRQn = 24,
    External_25_IRQn = 25,
    External_26_IRQn = 26,
    External_27_IRQn = 27,
    External_28_IRQn = 28,
    External_29_IRQn = 29,
    External_30_IRQn = 30,
    External_31_IRQn = 31
} IRQn_Type;

/*==============================================================================
 * PLIC: Platform Level Interrupt Controller
 */
#define PLIC_BASE_ADDR 0x40000000UL

typedef struct
{
    volatile uint32_t PRIORITY_THRESHOLD;
    volatile uint32_t CLAIM_COMPLETE;
    volatile uint32_t reserved[1022];
} IRQ_Target_Type;

typedef struct
{
    volatile uint32_t ENABLES[32];
} Target_Enables_Type;

typedef struct
{
    volatile uint32_t RESERVED0;
    
    /*-------------------- Source Priority --------------------*/
    volatile uint32_t SOURCE_PRIORITY[1023];
    
    /*-------------------- Pending array --------------------*/
    volatile const uint32_t PENDING_ARRAY[32];
    volatile uint32_t RESERVED1[992];
    
    /*-------------------- Target enables --------------------*/
    volatile Target_Enables_Type TARGET_ENABLES[15872];
    volatile uint32_t RESERVED2[32768];
    
    /*-------------------- Target enables --------------------*/
    IRQ_Target_Type TARGET[15872];
    
} PLIC_Type;

#define PLIC    ((PLIC_Type *)PLIC_BASE_ADDR)

/*==============================================================================
 * PRCI: Power, Reset, Clock, Interrupt
 */
#define PRCI_BASE   0x44000000UL

typedef struct
{
    volatile uint32_t MSIP[4095];
    volatile uint64_t MTIMECMP[4095];
    volatile const uint64_t MTIME;
} PRCI_Type;

#define PRCI    ((PRCI_Type *)PRCI_BASE) 

/*==============================================================================
 * Move the following to appropriate header.
 */
static inline void PLIC_init(void)
{
	uint32_t inc;
	unsigned long hart_id = read_csr(mhartid);

	/* Disable all interrupts for the current hart. */
	for(inc = 0; inc < ((PLIC_NUM_SOURCES + 32u) / 32u); ++inc)
	{
		PLIC->TARGET_ENABLES[hart_id].ENABLES[inc] = 0;
	}

	/* Set priorities to zero. */
	/* Should this really be done??? Calling PLIC_init() on one hart will cause
	 * the priorities previously set by other harts to be messed up. */
	for(inc = 0; inc < PLIC_NUM_SOURCES; ++inc)
	{
        PLIC->SOURCE_PRIORITY[inc] = 0;
    }

	/* Set the threshold to zero. */
	PLIC->TARGET[hart_id].PRIORITY_THRESHOLD = 0;

	/* Enable machine external interrupts. */
	set_csr(mie, MIP_MEIP);
}

static inline void PLIC_EnableIRQ(IRQn_Type IRQn)
{
    unsigned long hart_id = read_csr(mhartid);
    uint32_t current = PLIC->TARGET_ENABLES[hart_id].ENABLES[IRQn / 32];
    current |= (uint32_t)1 << (IRQn % 32);
    PLIC->TARGET_ENABLES[hart_id].ENABLES[IRQn / 32] = current;
}


static inline void PLIC_DisableIRQ(IRQn_Type IRQn)
{
    unsigned long hart_id = read_csr(mhartid);
    uint32_t current = PLIC->TARGET_ENABLES[hart_id].ENABLES[IRQn / 32];
    current &= ~((uint32_t)1 << (IRQn % 32));
    PLIC->TARGET_ENABLES[hart_id].ENABLES[IRQn / 32] = current;
}


static inline void PLIC_SetPriority(IRQn_Type IRQn, uint32_t priority) 
{
    PLIC->SOURCE_PRIORITY[IRQn] = priority;
}


static inline uint32_t PLIC_GetPriority(IRQn_Type IRQn)
{
    return PLIC->SOURCE_PRIORITY[IRQn];
}


static inline uint32_t PLIC_ClaimIRQ(void)
{
	unsigned long hart_id = read_csr(mhartid);

	return PLIC->TARGET[hart_id].CLAIM_COMPLETE;
}


static inline void PLIC_CompleteIRQ(uint32_t source)
{
	unsigned long hart_id = read_csr(mhartid);

	PLIC->TARGET[hart_id].CLAIM_COMPLETE = source;
}

#endif  /* RISCV_COREPLEXE31_H */


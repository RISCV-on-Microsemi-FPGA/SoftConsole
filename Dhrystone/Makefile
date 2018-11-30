LINKER_SCRIPT := ./riscv_hal/microsemi-riscv-ram.ld
#LINKER_SCRIPT := all-ram.lds
#LINKER_SCRIPT := ./riscv_hal/all-ram-2.lds
#LINKER_SCRIPT := ./riscv_hal/link.lds

C_DEFINES = -DMSCC_STDIO_THRU_CORE_UART_APB 

TARGET = dhrystone

ASM_SRCS := 
C_SRCS := ./riscv_hal/init.c dhry_1.c dhry_2.c dhry_stubs.c dhry_printf.c
RISCV_HAL_ASM_SRCS := ./riscv_hal/entry.S
RISCV_HAL_C_SRCS := ./riscv_hal/syscall.c ./riscv_hal/riscv_hal.c ./riscv_hal/riscv_hal_stubs.c
HEADERS := ./riscv_hal/encoding.h ./riscv_hal/riscv_plic.h dhry.h 


DHRYSTONE_CFLAGS := -DTIME -fno-inline -Wno-implicit

C_SRCS += ./hal/hw_reg_access.c
C_SRCS += ./drivers/CoreGPIO/core_gpio.c
C_SRCS += ./drivers/CoreUARTapb/core_uart_apb.c
C_SRCS += ./drivers/CoreTimer/core_timer.c

INCLUDES += -I.
INCLUDES += -I./riscv_hal
INCLUDES += -I./drivers/CoreGPIO
INCLUDES += -I./drivers/CoreUARTapb
INCLUDES += -I./drivers/CoreTimer
INCLUDES += -I./hal


CC := riscv64-unknown-elf-gcc
CFLAGS := $(DHRYSTONE_CFLAGS) -O0 -g -march=rv32im -mabi=ilp32 -fno-common -fno-builtin-printf -fno-toplevel-reorder $(INCLUDES) $(C_DEFINES)
LDFLAGS := -T $(LINKER_SCRIPT) -nostdlib -nostartfiles -lc -lgcc #--specs=nano.specs
LDFLAGS += -L./riscv_hal

ASM_OBJS := $(patsubst %.S,%.o,$(ASM_SRCS))
RISCV_HAL_ASM_OBJS := $(patsubst %.S,%.o,$(RISCV_HAL_ASM_SRCS))
C_OBJS := $(patsubst %.c,%.o,$(C_SRCS))
RISCV_HAL_C_OBJS := $(patsubst %.c,%.o,$(RISCV_HAL_C_SRCS))

$(TARGET).elf: $(ASM_OBJS) $(RISCV_HAL_ASM_OBJS) $(C_OBJS) $(RISCV_HAL_C_OBJS) $(LINKER_SCRIPT)
	$(CC) $(CFLAGS) $(ASM_OBJS) $(C_OBJS) $(RISCV_HAL_ASM_OBJS) $(RISCV_HAL_C_OBJS) -o $@ $(LDFLAGS)

$(ASM_OBJS): %.o: %.S $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(RISCV_HAL_ASM_OBJS): %.o: %.S $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(C_OBJS): %.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(RISCV_HAL_C_OBJS): %.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

ELF_TO_IHEX = riscv64-unknown-elf-objcopy -O ihex --set-start 0
ELF_TO_LST = -riscv64-unknown-elf-objdump --source --all-headers --demangle --line-numbers --wide "dhrystone.elf" > "dhrystone.lst"


dhrystone.lst: dhrystone.elf
	$(ELF_TO_LST) 
	
default: $(TARGET).elf

all: default $(TARGET).lst 

clean:
	rm -f dhrystone *.o ./riscv_hal/*.o dhrystone.lst dhrystone.elf

.PHONY: clean all default

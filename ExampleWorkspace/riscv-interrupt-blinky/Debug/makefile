################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include riscv_hal/subdir.mk
-include hal/subdir.mk
-include drivers/CoreUARTapb/subdir.mk
-include drivers/CoreTimer/subdir.mk
-include drivers/CoreGPIO/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
riscv-interrupt-blinky.hex \

SECONDARY_LIST += \
riscv-interrupt-blinky.lst \

SECONDARY_SIZE += \
riscv-interrupt-blinky.siz \


# All Target
all: riscv-interrupt-blinky.elf secondary-outputs

# Tool invocations
riscv-interrupt-blinky.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: RISC-V GCC/Newlib C Linker'
	riscv64-unknown-elf-gcc -mabi=ilp32 -g3 -gdwarf-2 -march=rv32im -T"../riscv_hal/microsemi-riscv-ram.ld" -nostartfiles -Wl,--gc-sections -Wl,-Map,riscv-interrupt-blinky.map -o "riscv-interrupt-blinky.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

riscv-interrupt-blinky.hex: riscv-interrupt-blinky.elf
	@echo 'Invoking: RISC-V GCC/Newlib GNU Create Flash Image'
	riscv64-unknown-elf-objcopy -O ihex riscv-interrupt-blinky.elf  "riscv-interrupt-blinky.hex"
	@echo 'Finished building: $@'
	@echo ' '

riscv-interrupt-blinky.lst: riscv-interrupt-blinky.elf
	@echo 'Invoking: RISC-V GCC/Newlib Create Listing'
	riscv64-unknown-elf-objdump -h -S riscv-interrupt-blinky.elf > "riscv-interrupt-blinky.lst"
	@echo 'Finished building: $@'
	@echo ' '

riscv-interrupt-blinky.siz: riscv-interrupt-blinky.elf
	@echo 'Invoking: RISC-V GCC/Newlib Print Size'
	riscv64-unknown-elf-size --format=berkeley "riscv-interrupt-blinky.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS) riscv-interrupt-blinky.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

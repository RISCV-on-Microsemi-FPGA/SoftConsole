################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../hal/hal_irq.c \
../hal/hw_reg_access.c 

OBJS += \
./hal/hal_irq.o \
./hal/hw_reg_access.o 

C_DEPS += \
./hal/hal_irq.d \
./hal/hw_reg_access.d 


# Each subdirectory must supply rules for building sources it contributes
hal/%.o: ../hal/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: RISC-V GCC/Newlib C Compiler'
	riscv64-unknown-elf-gcc -mabi=ilp32 -g3 -gdwarf-2 -march=rv32im -I"C:\Users\cyril.jean\Documents\GitHub\SoftConsole\ExampleWorkspace\riscv-interrupt-blinky" -I"C:\Users\cyril.jean\Documents\GitHub\SoftConsole\ExampleWorkspace\riscv-interrupt-blinky\drivers\CoreGPIO" -I"C:\Users\cyril.jean\Documents\GitHub\SoftConsole\ExampleWorkspace\riscv-interrupt-blinky\drivers\CoreTimer" -I"C:\Users\cyril.jean\Documents\GitHub\SoftConsole\ExampleWorkspace\riscv-interrupt-blinky\drivers\CoreUARTapb" -I"C:\Users\cyril.jean\Documents\GitHub\SoftConsole\ExampleWorkspace\riscv-interrupt-blinky\hal" -I"C:\Users\cyril.jean\Documents\GitHub\SoftConsole\ExampleWorkspace\riscv-interrupt-blinky\riscv_hal" -O0 -ffunction-sections -fdata-sections -Wall -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/CoreTimer/core_timer.c 

OBJS += \
./drivers/CoreTimer/core_timer.o 

C_DEPS += \
./drivers/CoreTimer/core_timer.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/CoreTimer/%.o: ../drivers/CoreTimer/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: RISC-V GCC/Newlib C Compiler'
	riscv64-unknown-elf-gcc -mabi=ilp32 -g3 -gdwarf-2 -march=rv32im -I"/scratch/MSCC_github_tests/SoftConsole/ExampleWorkspace/riscv-interrupt-blinky" -I"/scratch/MSCC_github_tests/SoftConsole/ExampleWorkspace/riscv-interrupt-blinky/drivers/CoreGPIO" -I"/scratch/MSCC_github_tests/SoftConsole/ExampleWorkspace/riscv-interrupt-blinky/drivers/CoreTimer" -I"/scratch/MSCC_github_tests/SoftConsole/ExampleWorkspace/riscv-interrupt-blinky/drivers/CoreUARTapb" -I"/scratch/MSCC_github_tests/SoftConsole/ExampleWorkspace/riscv-interrupt-blinky/hal" -I"/scratch/MSCC_github_tests/SoftConsole/ExampleWorkspace/riscv-interrupt-blinky/riscv_hal" -O0 -ffunction-sections -fdata-sections -Wall -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



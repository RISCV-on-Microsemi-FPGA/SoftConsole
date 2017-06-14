################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/CoreUARTapb/core_uart_apb.c 

OBJS += \
./drivers/CoreUARTapb/core_uart_apb.o 

C_DEPS += \
./drivers/CoreUARTapb/core_uart_apb.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/CoreUARTapb/%.o: ../drivers/CoreUARTapb/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: RISC-V GCC/Newlib C Compiler'
	riscv64-unknown-elf-gcc -mabi=ilp32 -g3 -gdwarf-2 -march=rv32im -I"/scratch/MSCC_github_tests/SoftConsole/ExampleWorkspace/riscv-systick-blinky" -I"/scratch/MSCC_github_tests/SoftConsole/ExampleWorkspace/riscv-systick-blinky/drivers/CoreGPIO" -I"/scratch/MSCC_github_tests/SoftConsole/ExampleWorkspace/riscv-systick-blinky/drivers/CoreUARTapb" -I"/scratch/MSCC_github_tests/SoftConsole/ExampleWorkspace/riscv-systick-blinky/hal" -I"/scratch/MSCC_github_tests/SoftConsole/ExampleWorkspace/riscv-systick-blinky/riscv_hal" -O0 -ffunction-sections -fdata-sections -Wall -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



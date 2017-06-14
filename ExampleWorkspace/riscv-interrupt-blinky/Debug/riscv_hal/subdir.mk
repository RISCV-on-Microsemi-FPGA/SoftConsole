################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../riscv_hal/init.c \
../riscv_hal/riscv_hal.c \
../riscv_hal/riscv_hal_stubs.c \
../riscv_hal/syscall.c 

S_UPPER_SRCS += \
../riscv_hal/entry.S 

OBJS += \
./riscv_hal/entry.o \
./riscv_hal/init.o \
./riscv_hal/riscv_hal.o \
./riscv_hal/riscv_hal_stubs.o \
./riscv_hal/syscall.o 

S_UPPER_DEPS += \
./riscv_hal/entry.d 

C_DEPS += \
./riscv_hal/init.d \
./riscv_hal/riscv_hal.d \
./riscv_hal/riscv_hal_stubs.d \
./riscv_hal/syscall.d 


# Each subdirectory must supply rules for building sources it contributes
riscv_hal/%.o: ../riscv_hal/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: RISC-V GCC/Newlib Assembler'
	riscv64-unknown-elf-gcc -mabi=ilp32 -g3 -gdwarf-2 -march=rv32im -x assembler-with-cpp -Wall -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -Wa,-march=rv32im -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

riscv_hal/%.o: ../riscv_hal/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: RISC-V GCC/Newlib C Compiler'
	riscv64-unknown-elf-gcc -mabi=ilp32 -g3 -gdwarf-2 -march=rv32im -I"/scratch/MSCC_github_tests/SoftConsole/ExampleWorkspace/riscv-interrupt-blinky" -I"/scratch/MSCC_github_tests/SoftConsole/ExampleWorkspace/riscv-interrupt-blinky/drivers/CoreGPIO" -I"/scratch/MSCC_github_tests/SoftConsole/ExampleWorkspace/riscv-interrupt-blinky/drivers/CoreTimer" -I"/scratch/MSCC_github_tests/SoftConsole/ExampleWorkspace/riscv-interrupt-blinky/drivers/CoreUARTapb" -I"/scratch/MSCC_github_tests/SoftConsole/ExampleWorkspace/riscv-interrupt-blinky/hal" -I"/scratch/MSCC_github_tests/SoftConsole/ExampleWorkspace/riscv-interrupt-blinky/riscv_hal" -O0 -ffunction-sections -fdata-sections -Wall -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



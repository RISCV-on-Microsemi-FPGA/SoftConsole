================================================================================
                    System Internal Timer example project
================================================================================

This SoftConsole example project demonstrates how to configure and use the 
CoreRISCV_AXI4 system timer. 

--------------------------------------------------------------------------------
                            CoreRISCV_AXI4 Soft Processor
--------------------------------------------------------------------------------
This example uses CoreRISCV_AXI4 SoftProcessor (RISC-V RV32IM). This design is 
built for debugging CoreRISCV_AXI4 through the SmartFusion2 FPGA programming 
JTAG port using a FlashPro5. To achieve this the CoreJTAGDebug IP is used to 
connect to the JTAG port of the CoreRISCV_AXI4.

Optionally, The design can be build to use Olimex ARM-USB-TINY-H JTAG probe. 
For this,The JTAG pins must be routed through Fabric to the top level pins.

All the platform/design specific definitions such as peripheral base addresses,
system clock frequency etc. are included in hw_platform.h. The hw_platform.h is 
located at the root folder of this project.

The CoreRISCV_AXI4 firmware projects needs the riscv_hal and the hal firmware
(RISC-V HAL).

The RISC-V HAL is available through Firmware catalog as well as the link below:
    https://github.com/RISCV-on-Microsemi-FPGA/riscv-hal

--------------------------------------------------------------------------------
                            How to use this example
--------------------------------------------------------------------------------
The System timer is configured using SysTick_Config() function. This function
also enables the internal system timer interrupt. This example implements the
SysTick_Handler() function which will be called by the RISCV-HAL.The application
specific interrupt handling is implemented in SysTick_Handler() function.
As demonstration, the LEDs blink pattern can be observed to see the System internal
timer interrupt functionality.

This example project requires USB-UART interface to be connected to a host PC. 
The host PC must connect to the serial port using a terminal emulator such as
HyperTerminal or PuTTY configured as follows:
    - 115200 baud
    - 8 data bits
    - 1 stop bit
    - no parity
    - no flow control

--------------------------------------------------------------------------------
                                Target hardware
--------------------------------------------------------------------------------
This example project is targeted at a SmartFusion2 M2S150 advanced development kit
design which has CoreTimer enabled. 
The example project is built using a clock frequency of 83MHz. Trying to execute 
this example project on a different design will result in incorrect baud rate 
being used by CoreUART and timer load value.

This example project can be used with another design using a different clock
configuration. This can be achieved by overwriting the content of this example
project's "hw_platform.h" file with the correct data from your Libero design.

The release mode configuration for this example project uses run_from_nvm.ld 
linker script. This Linker scripts is specific for the SmartFusion2 target. It 
creates the executable image for the eNVM memory area. To use this release mode 
configuration executable (.hex) on SmartFusion2, you need to attach it as an eNVM 
client in your Libero design.

An example design for SmartFusion2 150 Ad. Dev Kit is available at 
https://github.com/RISCV-on-Microsemi-FPGA/M2S150-Advanced-Dev-Kit/tree/master/Libero/CoreRISCV_AXI4_BaseDesign

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                          Silicon revision dependencies
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
This example is tested on M2S150 device.

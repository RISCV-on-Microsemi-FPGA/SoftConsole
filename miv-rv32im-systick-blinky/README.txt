================================================================================
                    System Internal Timer example project
================================================================================

This SoftConsole example project demonstrates how to configure and use the 
Mi-V Soft processor system timer. 


--------------------------------------------------------------------------------
                            Mi-V Soft processor
--------------------------------------------------------------------------------
This example uses a Mi-V Soft processor MiV_RV32IM_L1_AHB. The design is 
built for debugging MiV_RV32IM_L1_AHB through the SmartFusion2 FPGA programming 
JTAG port using a FlashPro5. To achieve this the CoreJTAGDebug IP is used to 
connect to the JTAG port of the MiV_RV32IM_L1_AHB.

Optionally, The design can be build to use Olimex ARM-USB-TINY-H JTAG probe. 
For this,The JTAG pins must be routed through Fabric to the top level pins.

All the platform/design specific definitions such as peripheral base addresses,
system clock frequency etc. are included in hw_platform.h. The hw_platform.h is 
located at the root folder of this project.

The MiV_RV32IM_L1_AHB firmware projects needs the riscv_hal and the hal firmware
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

This example displays a message on HyperTerminal then echoes back characters
typed in HyperTerminal.


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

An example design for SmartFusion2 150 Ad. Dev Kit is available at 
https://github.com/RISCV-on-Microsemi-FPGA/M2S150-Advanced-Dev-Kit/tree/master/Libero/

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                          Silicon revision dependencies
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
This example is tested on M2S150 device.

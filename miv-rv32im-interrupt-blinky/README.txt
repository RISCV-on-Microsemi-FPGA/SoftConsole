================================================================================
                    CoreTIMER and external interrupt example project
================================================================================

This SoftConsole example project demonstrates how to use the CoreTimer driver.
This example project also demonstrates how to configure PLIC of Mi-V Soft processor 
to handle the external interrupts.

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
The timer0 and timer1 are enabled and are setup up in continuous mode. The
LEDs blinking on the board indicate the timer interrupt operation.

The PLIC enables the external interrupt handling on Mi-V Soft processor. Up to 31
external interrupts can be connected to Mi-V Soft processor. These external interrupts
are configured and handled via PLIC.

This example project demonstrates how to use PLIC functions to configure PLIC.
It also demonstrates how to configure and use an external interrupt handler.

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

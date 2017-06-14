================================================================================
                    CoreTIMER and external interrupt example project
================================================================================

This SoftConsole example project demonstrates how to use the CoreTimer driver.
This example project also demonstrates how to configure PLIC of CoreRISCV_AXI4 
to handle the external interrupts.

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
The timer0 and timer1 are enabled and are setup up in continuous mode. The
LEDs blinking on the board indicate the timer interrupt operation.

The PLIC enables the external interrupt handling on CoreRISCV_AXI4. Up to 31
external interrupts can be connected to CoreRISCV_AXI4. These external interrupts
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

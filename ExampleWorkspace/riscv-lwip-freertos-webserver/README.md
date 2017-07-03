## FreeRTOS+lwip Webserver example for Microsemi RISC-V

### Test Platform and FPGA design:
M2S150-Advanced-Dev-Kit

### How to run the FreeRTOS RISC-V port:
To know how to use the SoftConsole workspace, please refer the [Readme.md](https://github.com/RISCV-on-Microsemi-FPGA/SoftConsole/blob/master/ExampleWorkspace/README.md)

The riscv-freertos-sample is a self contained project. This project demonstrates 
the FreeRTOS+lwip webserver example running with Microsemi RISC-V processor. 
This project creates a simple webserver running on SmartFusion2 platform.

To use this example:
 * Connect the board to a DHCP capable switch or network.
 * Connect the board's USB-UART interface to a PC running a terminal emulation
   application such as HyperTerminal or PuTTY.
 * Execute the example software project.
 
####IP address assignment
This example project can be built to use a dynamically assigned IP address using
DHCP or a static IP address. You need to ensure that the defined symbol
NET_USE_DHCP is defined in your project settings if you wish to use DHCP.
Otherwise, please refer to function prvEthernetConfigureInterface() in "main.c"
to select the static IP address you wish to use.

You can use this IP address to access the web-page supported by this webserver.
The webpage displays the ethernet interface port parameter details.
The CoreTSE_AHB IP is used as MAC. This webpage also displays the transmit and
receive statistics of the CoreTSE_AHB.

This example project requires USB-UART interface to be connected to a host PC. 
The host PC must connect to the serial port using a terminal emulator such as 
TeraTerm or PuTTY configured as follows:
    
        - 115200 baud
        - 8 data bits
        - 1 stop bit
        - no parity
        - no flow control
    
The ./hw_platform.h file contains the design related information that is required 
for this project. If you update the design, the hw_platform.h must be updated 
accordingly.

This project is intended as the starting point for the users to use ethernet
stack with Microsemi's RISC-V processor. 
    
### Microsemi SoftConsole Toolchain:
To know more please refer: [SoftConsole](https://github.com/RISCV-on-Microsemi-FPGA/SoftConsole)

### Documentation for Microsemi RISC-V processor, SoftConsole toochain, Debug Tools, FPGA design etc.
To know more please refer: [Documentation](https://github.com/RISCV-on-Microsemi-FPGA/Documentation)
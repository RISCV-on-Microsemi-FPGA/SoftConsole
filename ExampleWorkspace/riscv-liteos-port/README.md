## Microsemi RISC-V port for Huawei LiteOS

### LiteOS
The LiteOS is available at: [LiteOS_Kernel](https://github.com/LITEOS/LiteOS_Kernel)

    
### Test Platform and FPGA design:
M2S150-Advanced-Dev-Kit
[SmartFusion2 Advanced Development Kit RISC-V Sample Design](https://github.com/RISCV-on-Microsemi-FPGA/M2S150-Advanced-Dev-Kit/blob/master/Programming_The_Target_Device/PROC_SUBSYSTEM_BaseDesign)   

### LiteOS Port Test results:
    LOS_BoadExampleEntry    -- executes on board.
    LOS_Inspect_Entry       -- Following tests are passed.
        LOS_INSPECT_TASK
        LOS_INSPECT_EVENT
        LOS_INSPECT_SEM
        LOS_INSPECT_MUTEX
        LOS_INSPECT_SYSTIC
        LOS_INSPECT_TIMER
        LOS_INSPECT_SMEM
        LOS_INSPECT_DMEM

### How to run the LiteOS RISC-V port:
Open SoftConsole IDE 
Import the project riscv-liteos-port into SoftConsole. 
Buld the project and DownLoad the from SoftConsole to M2S150-Advanced-Dev-Kit using FlashPro Debugger.

The riscv-liteos-port is a self contained project where only those LiteOS components 
which are related to RISC-V port are part of the project.
    
This example project requires USB-UART interface to be connected to a host PC. 
The host PC must connect to the serial port using a terminal emulator such as 
TeraTerm or PuTTY configured as follows:
    
        - 115200 baud
        - 8 data bits
        - 1 stop bit
        - no parity
        - no flow control
    
The platform/M2S150_RV32/hw_platform.h file contains the design related information
that is required for this project. If you update the design, the hw_platform.h 
must be updated accordingly.
    
The preprocessor LOS_M2S150_RV32 must be defined for this project.
     
### Microsemi RISC-V Port:
    Folders Added:
        \kernel\cpu\riscv
        \platform\M2S150_RV32
        \projects\M2S150_RV32_SoftConsole

    Files Updated:
        kernel\config\los_config.h
        kernel\link\gcc\los_builddef.h
        example\include\los_demo_debug.h
        example\api\los_inspect_entry.c

### Microsemi SoftConsole Toolchain:
To know more please refer: [SoftConsole](https://github.com/RISCV-on-Microsemi-FPGA/SoftConsole)

### Documentation for Microsemi RISC-V processor, SoftConsole toochain, Debug Tools, FPGA design etc.
To know more please refer: [Documentation](https://github.com/RISCV-on-Microsemi-FPGA/Documentation)
    

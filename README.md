# SoftConsole

## Overview

[SoftConsole](https://www.microsemi.com/products/fpga-soc/design-resources/design-software/softconsole) is Microsemi's free [Eclipse](https://www.eclipse.org/home/index.php)/[CDT](https://www.eclipse.org/cdt/) and [GNU MCU Eclipse](https://gnu-mcu-eclipse.github.io/) based Integrated Development Environment (IDE) provided as key part of the [Microsemi Mi-V Embedded Ecosystem](https://www.microsemi.com/products/fpga-soc/mi-v-ecosystem). SoftConsole supports development and debugging of [bare metal](https://en.wikipedia.org/wiki/Bare_machine) and [RTOS](https://en.wikipedia.org/wiki/Real-time_operating_system) based [RISC-V](https://riscv.org/) and [Arm Cortex-M](https://www.arm.com/products/processors/cortex-m) software in C, C++ and assembler using industry standard [GNU](https://www.gnu.org/) tools including [GCC](https://gcc.gnu.org/), [GDB](https://www.gnu.org/software/gdb/), [binutils](https://www.gnu.org/software/binutils/) running on Windows and Linux. SoftConsole supports embedded debugging over [JTAG](https://en.wikipedia.org/wiki/JTAG) using the [Microsemi FlashPro programmer](https://www.microsemi.com/products/fpga-soc/design-resources/programming-debug-tools) and [OpenOCD](http://openocd.org/).

The current released version of SoftConsole is v5.2 and can be downloaded from [here](https://www.microsemi.com/products/fpga-soc/design-resources/design-software/softconsole#downloads).
Please read the SoftConsole release notes in full before installing and using it.

## Example projects

This git repository includes some example projects for use with SoftConsole v5.2 (or later).
The [EGit Eclipse plugin](https://www.eclipse.org/egit/) bundled with SoftConsole can be used to retrieve these projects as follows:

1. Run SoftConsole and open a workspace
2. Select File > Import… > Git > Projects from Git
3. Click Next >
4. If the repository has not yet been cloned to a local copy...
   1. Select Clone URI
   2. Click Next >
   3. In the Location > URI field enter the repository address (e.g. https://github.com/RISCV-on-Microsemi-FPGA/SoftConsole) 
   4. The Host and Repository path fields should fill out automatically
   5. Click Next >
   6. Select the required branch (usually master)
   7. Click Next >
   8. In the Directory field enter the directory into which the repository will be cloned
   9. Click Next >
   10. The repository will now be cloned into the specified directory
5. If the repository has already been cloned to a local copy...
   1. Select Existing local repository
   2. Click Next >
   3. Click Add… and then use Browse… or Search to specify the folder containing the local git repository
   4. Click Next > 
6. Select Import existing Eclipse projects 
7. Click Next >
8. Check the Projects to be imported 
9. Click Finish
10. The projects will now be imported - note that they are imported into the current workspace but the project files remain in the cloned repository location and are not copied into the workspace folder

Alternatively git can be used from outside of SoftConsole and the projects imported into the workspace.

## Migrating from SoftConsole v5.1 to v5.2

SoftConsole v5.1 uses the [Roa Logic Eclipse Plugin for RISC-V GNU Toolchain](https://github.com/RoaLogic/riscv_gnu_eclipse) for RISC-V development support.
SoftConsole v5.2 uses the [GNU MCU Eclipse plugins](https://gnu-mcu-eclipse.github.io/) for RISC-V and Cortex-M development support.
Because different plugins are used, RISC-V projects from v5.1 are not compatible with v5.2 (and vice versa) and when opened all project properties will appear blank and the project will not build.

In order to migrate a RISC-V project from SoftConsole v5.1 to v5.2 follow these instructions:

1. Start with a known good SoftConsole v5.2 example project
   1. Run the Firmware Catalog
   2. If necessary download the RISC-V Hardware Abstraction Layer (HAL) v2.1.101 (or later) firmware core from the remote Firmware repository to the local vault
   3. Right click on the RISC-V Hardware Abstraction Layer (HAL) and select Generate sample project > RISC-V > SoftConsole v5.2 > Systick Timer Example
   4. Select a target folder and generate the example project into it
2. Import the example project into the SoftConsole v5.2 workspace
   1. Run SoftConsole and open/launch the required workspace
   2. Select File > Import… > General > Existing Projects into Workspace > 
   3. Click Next >
   4. Click Browse… and navigate to the folder into which the project was generated above
   5. Ensure that the required example project is checked in the Projects list
   6. Ensure that Options > Copy projects into workspace is checked
   7. Click Finish to import the project into the SoftConsole workspace
3. Ensure that the project builds correctly
   1. In SoftConsole's Project Explorer right click on the project and select Build Configurations > Build All
   2. Check that the Debug and Release configurations built correctly without any errors
   3. In Project Explorer select the project's Debug and Release folders and delete them
4. Rename the project
   1. Right click on the project in SoftConsole's Project Explorer
   2. Select Rename… and enter your preferred project name
5. Add Microsemi firmware drivers to the project
   1. If any of the firmware drivers in the original example project are NOT required then select the relevant folders in the project and delete them
   2. Identify which Microsemi firmware drivers are used in the original SoftConsole v5.1 project
   3. Run the Firmware Catalog
   4. Select the relevant firmware drivers 
   5. Right click on the selected drivers and select Generate…
   6. In the Generate Options dialog browse to the new project's top level folder
   7. Click Generate to generate the drivers into the project folder
   8. In SoftConsole right click on the project in Project Explorer and select Refresh
   9. Check that the newly generated drivers appear in the Project Explorer view for the project
6. Copy non Microsemi firmware core source files
   1. Run SoftConsole v5.1 and open the workspace containing the project being converted
   2. If the project Debug and/or Release folders exist then select them and delete them
   3. Select all source files/folders EXCEPT the drivers, hal and riscv_hal folders, right click and select Copy
   4. In the SoftConsole v5.2 Project Explorer right click on the new project and select Paste to copy the folders/files into the new project
7. Linker script
   1. If the SoftConsole v5.1 project uses a RISC-V HAL example linker script unchanged then skip to step 8
   2. If the SoftConsole v5.1 project uses a modified RISC-V HAL example linker script or some other custom linker script then the modifications/customizations will need to be applied to the equivalent RISC-V HAL 2.1.101 (or later) example linker script
8. Project settings
   1. In the SoftConsole v5.1 Project Explorer right click on the project, select Properties and navigate to C/C++ Build > Settings > Tool Settings
   2. Do the same for the new project in SoftConsole v5.2
   3. Review the SoftConsole v5.1 project settings and apply the equivalent settings in the SoftConsole v5.2 project
   4. When configuration project build settings make sure to select Configuration = All Configurations for ones that are common to all configurations or else the specific configuration (usually Debug or Release) for those that apply to one configuration.
   5. The most common settings that need to be configured in the SoftConsole v5.2 project are
      - Target Processor: the layout of these settings and the options available differ between SoftConsole v5.1 and SoftConsole v5.2 but the key requirement is that the selected settings appropriately describe the target processor's bit size (32 or 64), extensions and ABI.
      - Include defines, paths and files: replicate the SoftConsole v5.1 GNU RISC-V GCC/Newlib C/C++ Compiler > Preprocessor and Includes options in the SoftConsole v5.2 GNU RISC-V Cross C/C++ Compiler > Preprocessor and Includes property pages.
      - Linker script: see above for information about SoftConsole v5.1 projects that use a modified or custom linker script. Configure the appropriate linker script in SoftConsole v5.2 under GNU RISC-V Cross C/C++ Linker > General > Script files (-T)
      - Newlib nano: Unlike SoftConsole v5.1, SoftConsole v5.2 comes with newlib nano which is a version of newlib optimized for use in resource constrained embedded environments. It is generally advisable to use this if possible. To do so ensure that the SoftConsole v5.1 GNU RISC-V Cross C/C++ Linker > Miscellaneous > Use newlib-nano (--specs=nano.specs) is checked.
9.	Build the new project
    1.	In SoftConsole v5.2 right click on the project in Project Explorer and select Build Configurations > Build All to check that all configurations build successfully
 
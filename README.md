# SoftConsole

## Overview

[SoftConsole](https://www.microsemi.com/products/fpga-soc/design-resources/design-software/softconsole) is Microsemi's free Eclipse based Integrated Development Environment (IDE) provided as part of the [Microsemi's Mi-V Embedded Ecosystem](https://www.microsemi.com/products/fpga-soc/mi-v-ecosystem). SoftConsole supports the development and debugging of [bare metal](https://en.wikipedia.org/wiki/Bare_machine) and RTOS based RISC-V and Cortex-M software using C, C++ and assembler.

SoftConsole supports embedded debugging over [JTAG](https://en.wikipedia.org/wiki/JTAG) using the [Microsemi FlashPro programmer](https://www.microsemi.com/products/fpga-soc/design-resources/programming-debug-tools).

The current released version of SoftConsole is v5.2.
Please read the SoftConsole release notes in full before installing and using it.

## Example projects

This git repository includes some example projects for use with SoftConsole v5.2 (or later).
The [EGit Eclipse plugin](https://www.eclipse.org/egit/) bundled with SoftConsole can be used to retrieve these projects as follows:

1.	Run SoftConsole and open a workspace
2.	Select File > Import… > Git > Projects from Git
3.	Click Next >
4.	If the repository has not yet been cloned to a local copy...
a.	Select Clone URI
b.	Click Next >
c.	In the Location > URI field enter the repository address (e.g. https://github.com/RISCV-on-Microsemi-FPGA/SoftConsole) 
d.	The Host and Repository path fields should fill out automatically
e.	Click Next >
f.	Select the required branch (usually master)
g.	Click Next >
h.	In the Directory field enter the directory into which the repository will be cloned
i.	Click Next >
j.	The repository will now be cloned into the specified directory
5.	If the repository has already been cloned to a local copy...
a.	Select Existing local repository
b.	Click Next >
c.	Click Add… and then use Browse… or Search to specify the folder containing the local git repository
d.	Click Next > 
6.	Select Import existing Eclipse projects 
7.	Click Next >
8.	Check the Projects to be imported 
9.	Click Finish
10.	The projects will now be imported - note that they are imported into the current workspace but the project files remain in the cloned repository location and are not copied into the workspace folder



## Migrating from SoftConsole v5.1 to v5.2

SoftConsole v5.1 uses the [Roa Logic Eclipse Plugin for RISC-V GNU Toolchain](https://github.com/RoaLogic/riscv_gnu_eclipse) for RISC-V development support.
SoftConsole v5.2 uses the [GNU MCU Eclipse plugins](https://gnu-mcu-eclipse.github.io/) for RISC-V and Cortex-M development support.
Because different plugins are used, RISC-V projects from v5.1 are not compatible with v5.2 (and vice versa) and when opened all project properties will appear blank and the project will not build.

In order to migrate a RISC-V project from SoftConsole v5.1 to v5.2 follow these instructions:

1.	Start with a known good SoftConsole v5.2 example project
a.	Run the Firmware Catalog
b.	If necessary download the RISC-V Hardware Abstraction Layer (HAL) v2.1.101 (or later) firmware core from the remote Firmware repository to the local vault
c.	Right click on the RISC-V Hardware Abstraction Layer (HAL) and select Generate sample project > RISC-V > SoftConsole v5.2 > Systick Timer Example
d.	Select a target folder and generate the example project into it
2.	Import the example project into the SoftConsole v5.2 workspace
a.	Run SoftConsole and open/launch the required workspace
b.	Select File > Import… > General > Existing Projects into Workspace > 
c.	Click Next >
d.	Click Browse… and navigate to the folder into which the project was generated above
e.	Ensure that the required example project is checked in the Projects list
f.	Ensure that Options > Copy projects into workspace is checked
g.	Click Finish to import the project into the SoftConsole workspace
3.	Ensure that the project builds correctly
a.	In SoftConsole's Project Explorer right click on the project and select Build Configurations > Build All
b.	Check that the Debug and Release configurations built correctly without any errors
c.	In Project Explorer select the project's Debug and Release folders and delete them
4.	Rename the project
a.	Right click on the project in SoftConsole's Project Explorer
b.	Select Rename… and enter your preferred project name
5.	Add Microsemi firmware drivers to the project
a.	If any of the firmware drivers in the original example project are NOT required then select the relevant folders in the project and delete them
b.	Identify which Microsemi firmware drivers are used in the original SoftConsole v5.1 project
c.	Run the Firmware Catalog
d.	Select the relevant firmware drivers 
e.	Right click on the selected drivers and select Generate…
f.	In the Generate Options dialog browse to the new project's top level folder
g.	Click Generate to generate the drivers into the project folder
h.	In SoftConsole right click on the project in Project Explorer and select Refresh

i.  Check that the newly generated drivers appear in the Project Explorer view for the project
6.	Copy non Microsemi firmware core source files
a.	Run SoftConsole v5.1 and open the workspace containing the project being converted
b.	If the project Debug and/or Release folders exist then select them and delete them
c.	Select all source files/folders EXCEPT the drivers, hal and riscv_hal folders, right click and select Copy
d.	In the SoftConsole v5.2 Project Explorer right click on the new project and select Paste to copy the folders/files into the new project
7.	Linker script
a.	If the SoftConsole v5.1 project uses a RISC-V HAL example linker script unchanged then skip to step 8
b.	If the SoftConsole v5.1 project uses a modified RISC-V HAL example linker script or some other custom linker script then the modifications/customizations will need to be applied to the equivalent RISC-V HAL 2.1.101 (or later) example linker script
8.	Project settings
a.	In the SoftConsole v5.1 Project Explorer right click on the project, select Properties and navigate to C/C++ Build > Settings > Tool Settings
b.	Do the same for the new project in SoftConsole v5.2
c.	Review the SoftConsole v5.1 project settings and apply the equivalent settings in the SoftConsole v5.2 project
d.	When configuration project build settings make sure to select Configuration = All Configurations for ones that are common to all configurations or else the specific configuration (usually Debug or Release) for those that apply to one configuration.
e.	The most common settings that need to be configured in the SoftConsole v5.2 project are
    * Target Processor: the layout of these settings and the options available differ between SoftConsole v5.1 and SoftConsole v5.2 but the key requirement is that the selected settings appropriately describe the target processor's bit size (32 or 64), extensions and ABI.
    * Include defines, paths and files: replicate the SoftConsole v5.1 GNU RISC-V GCC/Newlib C/C++ Compiler > Preprocessor and Includes options in the SoftConsole v5.2 GNU RISC-V Cross C/C++ Compiler > Preprocessor and Includes property pages.
    * Linker script: see above for information about SoftConsole v5.1 projects that use a modified or custom linker script. Configure the appropriate linker script in SoftConsole v5.2 under GNU RISC-V Cross C/C++ Linker > General > Script files (-T)
    * Newlib nano: Unlike SoftConsole v5.1, SoftConsole v5.2 comes with newlib nano which is a version of newlib optimized for use in resource constrained embedded environments. It is generally advisable to use this if possible. To do so ensure that the SoftConsole v5.1 GNU RISC-V Cross C/C++ Linker > Miscellaneous > Use newlib-nano (--specs=nano.specs) is checked.
9.	Build the new project
a.	In SoftConsole v5.2 right click on the project in Project Explorer and select Build Configurations > Build All to check that all configurations build successfully
 

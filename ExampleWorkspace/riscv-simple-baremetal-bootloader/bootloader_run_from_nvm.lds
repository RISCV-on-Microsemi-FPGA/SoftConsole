OUTPUT_ARCH( "riscv" )
ENTRY(_start)

STACK_SIZE = 128k;
HEAP_SIZE = 128k;

MEMORY
{
  nvm (rx)   : ORIGIN = 0x60000000, LENGTH = 256k
  ram (rwx)  : ORIGIN = 0x80100000, LENGTH = 256m
}


SECTIONS
{
  .text : 
  {
    ./riscv_hal/entry.o(.text.entry)
    *(.text.entry)
    *(.text)
  } >nvm

  .sdata : {
    __sdata_load = LOADADDR(.sdata);
    __sdata_start = .; 
    _gp = . + 0x800;
    *(.srodata.cst16) *(.srodata.cst8) *(.srodata.cst4) *(.srodata.cst2)
    *(.srodata*)
    *(.sdata .sdata.* .gnu.linkonce.s.*)
    __sdata_end = .;
  } >ram AT>nvm

  .data : ALIGN(0x10)
  {
    __data_load = LOADADDR(.data);
    __data_start = .; 
    *(.data)
    __data_end = .;
  } >ram AT>nvm

  /* bss segment */
  .sbss : {
    __sbss_start = .;
    *(.sbss .sbss.* .gnu.linkonce.sb.*)
    *(.scommon)
    __sbss_end = .;
  } >ram
  
  .bss :
  { 
    __bss_start = .;
    *(.bss)
    __bss_end = .;
  } >ram

  /* End of uninitalized data segement */
  _end = .;
  
  .heap : ALIGN(0x10)
  {
    __heap_start = .;
    . += HEAP_SIZE;
    __heap_end = .;
    _heap_end = __heap_end;
  } >ram
  
  .stack : ALIGN(0x10)
  {
    __stack_bottom = .;
    . += STACK_SIZE;
    __stack_top = .;
  } >ram
}



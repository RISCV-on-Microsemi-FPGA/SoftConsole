OUTPUT_ARCH( "riscv" )
ENTRY(_start)

STACK_SIZE = 128k;
HEAP_SIZE = 128k;

SECTIONS
{
  . = 0x80100000;
  .text : 
  {
    ./riscv_hal/entry.o(.text.entry)
    *(.text)
  }

  /* short/global data section */
  .sdata : ALIGN(0x10)
  {
    __sdata_load = LOADADDR(.sdata);
    __sdata_start = .; 
    _gp = . + 0x800;
    *(.srodata.cst16) *(.srodata.cst8) *(.srodata.cst4) *(.srodata.cst2)
    *(.srodata*)
    *(.sdata .sdata.* .gnu.linkonce.s.*)
    __sdata_end = .;
  }

  /* data section */
  .data : ALIGN(0x10)
  { 
    __data_load = LOADADDR(.data);
    __data_start = .; 
    *(.data)
    __data_end = .;
  }

  /* sbss section */
  .sbss : ALIGN(0x10)
  {
    __sbss_start = .;
    *(.sbss .sbss.* .gnu.linkonce.sb.*)
    *(.scommon)
    __sbss_end = .;
  }
  
  /* sbss section */
  .bss : ALIGN(0x10)
  { 
    __bss_start = .;
    *(.bss)
    __bss_end = .;
  }

  /* End of uninitalized data segement */
  _end = .;
  
  .heap : ALIGN(0x10)
  {
    __heap_start = .;
    . += HEAP_SIZE;
    __heap_end = .;
    _heap_end = __heap_end;
  }
  
  .stack : ALIGN(0x10)
  {
    __stack_bottom = .;
    . += STACK_SIZE;
    __stack_top = .;
  }
}

OUTPUT_ARCH( "riscv" )

SECTIONS
{
  . = 0x80000000;
  .text : 
  {
    entry.o(.text)
    *(.text)
  }

  /* data segment */
  _data = .;
  .data : { *(.data) }

  .sdata : {
    _gp = . + 0x800;
    *(.srodata.cst16) *(.srodata.cst8) *(.srodata.cst4) *(.srodata.cst2)
    *(.srodata*)
    *(.sdata .sdata.* .gnu.linkonce.s.*)
  }

  /* bss segment */
  .sbss : {
    *(.sbss .sbss.* .gnu.linkonce.sb.*)
    *(.scommon)
  }
  .bss : { *(.bss) }

  /* End of uninitalized data segement */
  _data_end = .;
  _end = .;
  _heap_end = .;
}


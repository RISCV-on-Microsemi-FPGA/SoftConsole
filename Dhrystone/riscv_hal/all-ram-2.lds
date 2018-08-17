OUTPUT_ARCH( "riscv" )

SECTIONS
{
  . = 0x80000000;
  .text :
  {
    entry.o(.text)
    dhry_stubs.o(.text)
    syscall.o(.text)
    *(.text.startup)
    *(.text)
  }

  .eh_frame : { *(.eh_frame) }

  .rodata : {
    *(.rodata*)
    *(.srodata.cst16) *(.srodata.cst8) *(.srodata.cst4) *(.srodata.cst2)
    *(.srodata*)
  }

  . = 0x80040000;
  _data = .;

  /* data segment */
  .data : { *(.data) }

  .sdata : {
    _gp = . + 0x800;
    *(.sdata .sdata.* .gnu.linkonce.s.*)
  }

  /* bss segment */
  .sbss : {
    *(.sbss .sbss.* .gnu.linkonce.sb.*)
    *(.scommon)
  }
  .bss : { *(.bss) }

  _data_end = .;
  _end = .;
}

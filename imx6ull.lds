SECTIONS {
    . = 0x87800000;

    .text ALIGN(4) :
    {
      *(.text)
    }

    .rodata ALIGN(4) : 
    { 
      *(.rodata) 
    }

    .data ALIGN(4) : 
    {
      *(.data) 
    }

    . = ALIGN(4);
    __bss_start = .;
    .bss ALIGN(4) : 
    { 
      *(.bss) *(.COMMON) 
    }
    __bss_end = .;
}

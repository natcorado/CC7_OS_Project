MEMORY {
    p2_code (rx) : ORIGIN = 0x80030000, LENGTH = 64K
    p2_data (rw) : ORIGIN = 0x80038000, LENGTH = 32K
}

SECTIONS {
    .text : {
        *(.text*)
        *(.rodata*)
    } > p2_code

    .data : {
        *(.data*)
    } > p2_data

    .bss : {
        *(.bss*)
        *(COMMON)
    } > p2_data

    .stack (NOLOAD) : {
        . = ALIGN(8);
        _stack_bottom = .;
        . += 0x2000;  /* 8KB stack */
        _stack_top = .;
    } > p2_data
}
MEMORY {
    os_code (rx) : ORIGIN = 0x80000000, LENGTH = 64K    /* OS code */
    os_data (rw) : ORIGIN = 0x80010000, LENGTH = 32K    /* OS data and stack */
}

SECTIONS {
    .text : {
        _start = .;
        *(.text*)
        *(.rodata*)
    } > os_code

    .data : {
        *(.data*)
    } > os_data

    .bss : {
        *(.bss*)
        *(COMMON)
    } > os_data

    .stack (NOLOAD) : {
        . = ALIGN(8);
        _stack_bottom = .;
        . += 0x2000;     
        _stack_top = .;
    } > os_data
}
ENTRY(start)

SECTIONS
{
    . = 0x8000;

    .text : {
        KEEP(*(.text))
    }

    .data : {
        *(.data)
        *(.rodata)
    }

    .bss : {
        *(.bss)
        *(COMMON)
    }
}

BITS 16

global start
extern kernel_main

start:
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7000

    call kernel_main

    cli
    hlt

BITS 16

global syscall_handler
extern syscall_dispatch

syscall_handler:
    push bp
    push si
    push di

    push dx
    push cx
    push bx
    push ax
    call syscall_dispatch
    add sp, 8

    pop di
    pop si
    pop bp
    iret

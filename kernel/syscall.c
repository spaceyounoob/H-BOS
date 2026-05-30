#include "syscall.h"
#include "floppy.h"

extern void syscall_handler(void);
void syscall_init(void) {
    unsigned int *ivt_offset  = (unsigned int *)(0x80 * 4);
    unsigned int *ivt_segment = (unsigned int *)(0x80 * 4 + 2);
    *ivt_offset  = (unsigned int)syscall_handler;
    *ivt_segment = 0x0000;
}

static void sys_print(const char *str) {
    while (*str) {
        char c = *str;
        __asm__ volatile (
            "movb $0x0E, %%ah\n\t"
            "int $0x10"
            : : "a"(c)
        );
        str++;
    }
}

void syscall_dispatch(unsigned int num, unsigned int arg1,
                      unsigned int arg2, unsigned int arg3) {
    switch (num) {
        case SYS_PRINT:
            sys_print((const char *)arg1);
            break;
        case SYS_READ_SECTOR:
            floppy_read(arg1, 1, arg2, arg3);
            break;
        default:
            break;
    }
}

#include "panic.h"

static void print_char(char c) {
    __asm__ volatile (
        "mov $0x0E, %%ah\n\t"
        "xor %%bh, %%bh\n\t"
        "int $0x10"
        : : "a"(c) : "bh"
    );
}

static void print_str(const char *s) {
    while (*s) print_char(*s++);
}

static void print_hex(unsigned short val) {
    const char *hex = "0123456789ABCDEF";
    print_char(hex[(val >> 12) & 0xF]);
    print_char(hex[(val >>  8) & 0xF]);
    print_char(hex[(val >>  4) & 0xF]);
    print_char(hex[(val      ) & 0xF]);
}

void panic(void) {
    unsigned short caller_bp;
    unsigned short ret_addr;

    __asm__ volatile ("mov %%bp, %0" : "=r"(caller_bp));
    ret_addr = *(unsigned short *)(caller_bp + 6);

    __asm__ volatile (
        "mov $0x06, %%ah\n\t"
        "mov $0x00, %%al\n\t"
        "mov $0x4F, %%bh\n\t"
        "mov $0x0000, %%cx\n\t"
        "mov $0x184F, %%dx\n\t"
        "int $0x10"
        : : : "ax", "bx", "cx", "dx"
    );

    __asm__ volatile (
        "mov $0x02, %%ah\n\t"
        "xor %%bh, %%bh\n\t"
        "xor %%dx, %%dx\n\t"
        "int $0x10"
        : : : "ax", "bx", "dx"
    );

    print_str("!!! PANIC !!!");
    print_str(" called from: 0x");
    print_hex(ret_addr);

    __asm__ volatile ("cli");
    while (1) {
        __asm__ volatile ("hlt");
    }
}
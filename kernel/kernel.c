#include "panic.h"
#include "syscall.h"
#define MAX_COMMAND_LEN 64

static void print_char(char c) {
    __asm__ volatile (
        "movb %0, %%al\n\t"
        "movb $0x0E, %%ah\n\t"
        "xorw %%bx, %%bx\n\t"
        "int $0x10\n\t"
        :
        : "q"(c)
        : "ax", "bx", "cc"
    );
}

static void print_str(const char *s) {
    while (*s) {
        print_char(*s++);
    }
}

static char get_char(void) {
    unsigned short ax_res;
    __asm__ volatile (
        "movb $0x00, %%ah\n\t"
        "int $0x16\n\t"
        : "=a"(ax_res)
        :
        : "cc"
    );
    return (char)(ax_res & 0xFF);
}

static void clear_screen(void) {
    __asm__ volatile (
        "movw $0x0003, %%ax\n\t"
        "int $0x10\n\t"
        :
        :
        : "ax", "cc"
    );
}


static int str_compare(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

static int str_starts_with(const char *str, const char *prefix) {
    while (*prefix) {
        if (*str != *prefix) {
            return 0;
        }
        str++;
        prefix++;
    }
    return 1;
}

void kernel_main(void) {
    char cmd_buf[MAX_COMMAND_LEN];
    int cmd_idx = 0;

    /* ig bro */
    clear_screen();
    print_str("HBOS 0.1 2026-2026\r\n");
   print_str("\n");
    print_str("$ ");

    while (1) {
        char c = get_char();

        if (c == '\r') {
            print_str("\r\n");
            cmd_buf[cmd_idx] = '\0';

            if (str_compare(cmd_buf, "cls") == 0) {
                clear_screen();

              /*  print_str("$ ");  */  }
                else if (str_starts_with(cmd_buf, "echo ")) {
                    print_str(&cmd_buf[5]);
                    print_str("\r\n");
                }

                else if (str_starts_with(cmd_buf, "help")) {
                    print_str("Available commands: echo, cls, help");
                    print_str("\r\n");
                }

                else if (str_compare(cmd_buf, "echo") == 0) {
                    print_str("\r\n");
                }
                else if (cmd_idx > 0) {
                    print_str("Invalid command ");
                    print_str(cmd_buf);
                    print_str(".\r\n");
                }

                cmd_idx = 0;
                print_str("$ ");
        }
        else if (c == '\b') {
            if (cmd_idx > 0) {
                cmd_idx--;
                print_char('\b');
                print_char(' ');
                print_char('\b');
            }
        }
        else {
            if (cmd_idx < MAX_COMMAND_LEN - 1) {
                cmd_buf[cmd_idx] = c;
                cmd_idx++;
                print_char(c);
            }
        }
    }
}


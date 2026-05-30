#include "floppy.h"

static void lba_to_chs(unsigned int lba,
                       unsigned char *cyl,
                       unsigned char *head,
                       unsigned char *sector) {
    *sector = (lba % FLOPPY_SECTORS_PER_TRACK) + 1;
    *head   = (lba / FLOPPY_SECTORS_PER_TRACK) % FLOPPY_HEADS;
    *cyl    = (lba / FLOPPY_SECTORS_PER_TRACK) / FLOPPY_HEADS;
                       }

                       int floppy_read(unsigned int lba, unsigned char count,
                                       unsigned short segment, unsigned short offset) {
                           unsigned char cyl, head, sector;
                           unsigned char err = 0;
                           lba_to_chs(lba, &cyl, &head, &sector);

                           __asm__ volatile (
                               "push %%es\n\t"
                               "mov %5, %%es\n\t"
                               "mov $0x02, %%ah\n\t"
                               "int $0x13\n\t"
                               "pop %%es\n\t"
                               "jnc 1f\n\t"
                               "movb $1, %0\n\t"
                               "1:"
                               : "=r"(err)
                               : "a"(count),
                                             "b"(offset),
                                             "c"((cyl << 8) | sector),
                                             "d"((head << 8) | 0x00),
                                             "r"(segment)
                                             : "memory"
                           );

                           return err;
                                       }

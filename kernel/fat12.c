#include "fat12.h"
#include "floppy.h"
#include "panic.h"
#include "string.h"

static unsigned char sector_buf[512];
static unsigned char fat_buf[512 * 2];

/* should i make a universal helpers file? yes! will I? no! ---coni */
static void debug_char(char c) {
    __asm__ volatile (
        "mov $0x0E, %%ah\n\t"
        "xor %%bh, %%bh\n\t"
        "int $0x10"
        : : "a"(c) : "bh"
    );
}

static void floppy_reset(void) {
    __asm__ volatile (
        "xor %%ah, %%ah\n\t"
        "xor %%dl, %%dl\n\t"
        "int $0x13"
        : : : "ax", "dl"
    );
}

static void read_sector(unsigned int lba, unsigned char *buf) {
    unsigned char retries = 3;
    unsigned short offset = (unsigned short)buf;

    while (retries--) {
        floppy_reset();
        if (!floppy_read(lba, 1, 0x0000, offset))
            return;
    }
}

static unsigned int fat12_next_cluster(unsigned int cluster) {
    unsigned int offset = cluster + (cluster / 2);
    unsigned int val = *(unsigned short *)(fat_buf + offset);
    if (cluster & 1)
        return val >> 4;
    else
        return val & 0x0FFF;
}

static int name_matches(const char *a, const char *b, int len) {
    int i;
    for (i = 0; i < len; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

int fat12_find_file(const char *name, const char *ext, DirEntry *out) {
    unsigned int sector, i;
    DirEntry *entries;

    for (sector = 0; sector < FAT12_ROOT_SECTORS; sector++) {
        read_sector(FAT12_ROOT_START + sector, sector_buf);
        entries = (DirEntry *)sector_buf;

        for (i = 0; i < 512 / sizeof(DirEntry); i++) {
            if (entries[i].name[0] == 0x00) return -1;
            if (entries[i].name[0] == 0xE5) continue;
            if (entries[i].attributes & 0x10) continue;

            if (name_matches((const char *)entries[i].name, name, 8) &&
                name_matches((const char *)entries[i].ext,  ext,  3)) {
                *out = entries[i];
                return 0;
            }
        }
    }
    return -1;
}

int fat12_load_file(DirEntry *entry, unsigned short segment, unsigned short offset) {
    unsigned int cluster = entry->first_cluster;
    unsigned short off = offset;

    read_sector(FAT12_FAT_START, fat_buf);
    read_sector(FAT12_FAT_START + 1, fat_buf + 512);

    while (cluster < 0xFF8) {
        unsigned int lba = FAT12_DATA_START + (cluster - 2);
        if (floppy_read(lba, 1, segment, off))
        off += FAT12_SECTOR_SIZE;
        cluster = fat12_next_cluster(cluster);
    }
    return 0;
}

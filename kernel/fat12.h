#ifndef FAT12_H
#define FAT12_H

#define FAT12_SECTOR_SIZE    512
#define FAT12_FAT_START      4
#define FAT12_FAT_SECTORS    2
#define FAT12_ROOT_START     8
#define FAT12_ROOT_SECTORS   7
#define FAT12_DATA_START     15

typedef struct {
    unsigned char  name[8];
    unsigned char  ext[3];
    unsigned char  attributes;
    unsigned char  reserved[10];
    unsigned short time;
    unsigned short date;
    unsigned short first_cluster;
    unsigned long  size;
} DirEntry;

int fat12_find_file(const char *name, const char *ext, DirEntry *out);
int fat12_load_file(DirEntry *entry, unsigned short segment, unsigned short offset);

#endif
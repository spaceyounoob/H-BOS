#ifndef FLOPPY_H
#define FLOPPY_H

/* 360kb geom */
#define FLOPPY_SECTORS_PER_TRACK 9
#define FLOPPY_HEADS             2
#define FLOPPY_CYLINDERS         40

int floppy_read(unsigned int lba, unsigned char count,
                unsigned short segment, unsigned short offset);

#endif

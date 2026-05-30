NASM     = nasm
CC       = ia16-elf-gcc
LD       = ia16-elf-ld
OBJCOPY  = ia16-elf-objcopy

CFLAGS   = -mtune=i8086 -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-omit-frame-pointer -O2

IMG      = os.img

all: $(IMG)
	
boot/boot.bin: boot/boot.asm
	$(NASM) -f bin -o $@ $^

kernel/entry.o: kernel/entry.asm
	$(NASM) -f elf -o $@ $^

kernel/isr.o: kernel/isr.asm
	$(NASM) -f elf -o $@ $^

kernel/kernel.o: kernel/kernel.c
	$(CC) $(CFLAGS) -c -o $@ $^

kernel/syscall.o: kernel/syscall.c
	$(CC) $(CFLAGS) -c -o $@ $^

kernel/floppy.o: kernel/floppy.c
	$(CC) $(CFLAGS) -c -o $@ $^

kernel/panic.o: kernel/panic.c
	$(CC) $(CFLAGS) -c -o $@ $^

kernel/fat12.o: kernel/fat12.c
	$(CC) $(CFLAGS) -c -o $@ $^

kernel/string.o: kernel/string.c
	$(CC) $(CFLAGS) -c -o $@ $^

kernel/kernel.elf: kernel/entry.o kernel/isr.o kernel/syscall.o kernel/floppy.o kernel/panic.o kernel/fat12.o kernel/string.o kernel/kernel.o
	$(LD) -T linker.ld -o $@ $^

kernel/kernel.bin: kernel/kernel.elf
	$(OBJCOPY) -O binary $< $@

$(IMG): boot/boot.bin kernel/kernel.bin 
	dd if=/dev/zero          of=$(IMG) bs=512 count=720
	mformat -i $(IMG) -f 360 -M 512 -R 4 ::
	dd if=boot/boot.bin      of=$(IMG) conv=notrunc
	dd if=kernel/kernel.bin  of=$(IMG) bs=512 seek=1 conv=notrunc

clean:
	rm -f boot/boot.bin kernel/*.o kernel/*.elf kernel/*.bin $(IMG)

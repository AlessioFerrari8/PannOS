CC = i686-elf-gcc
CFLAGS = -std=gnu11 -ffreestanding -O2 -Wall -Wextra
LDFLAGS = -ffreestanding -O2 -nostdlib

all: pannos.iso

kernel/boot.o: kernel/boot.s
	nasm -f elf32 kernel/boot.s -o kernel/boot.o

kernel/kernel.o: kernel/kernel.c
	$(CC) $(CFLAGS) -c kernel/kernel.c -o kernel/kernel.o

pannos.bin: kernel/boot.o kernel/kernel.o kernel/linker.ld
	$(CC) -T kernel/linker.ld -o pannos.bin $(LDFLAGS) kernel/boot.o kernel/kernel.o -lgcc

pannos.iso: pannos.bin grub.cfg
	mkdir -p isodir/boot/grub
	cp pannos.bin isodir/boot/pannos.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o pannos.iso isodir

run: pannos.iso
	qemu-system-i386 -cdrom pannos.iso

clean:
	rm -rf kernel/*.o pannos.bin pannos.iso isodir

CC = i686-elf-gcc
CFLAGS = -std=gnu11 -ffreestanding -O2 -Wall -Wextra
LDFLAGS = -ffreestanding -O2 -nostdlib

all: pannos.iso

kernel/boot.o: kernel/boot.s
	nasm -f elf32 kernel/boot.s -o kernel/boot.o

kernel/gdt_flush.o: kernel/gdt_flush.s
	nasm -f elf32 kernel/gdt_flush.s -o kernel/gdt_flush.o

kernel/idt_flush.o: kernel/idt_flush.s
	nasm -f elf32 kernel/idt_flush.s -o kernel/idt_flush.o

kernel/isr_stubs.o: kernel/isr_stubs.s
	nasm -f elf32 kernel/isr_stubs.s -o kernel/isr_stubs.o

kernel/isr.o: kernel/isr.c kernel/isr.h kernel/vga.h
	$(CC) $(CFLAGS) -c kernel/isr.c -o kernel/isr.o

kernel/kernel.o: kernel/kernel.c kernel/vga.h kernel/gdt.h kernel/idt.h
	$(CC) $(CFLAGS) -c kernel/kernel.c -o kernel/kernel.o

kernel/vga.o: kernel/vga.c kernel/vga.h
	$(CC) $(CFLAGS) -c kernel/vga.c -o kernel/vga.o

kernel/gdt.o: kernel/gdt.c kernel/gdt.h
	$(CC) $(CFLAGS) -c kernel/gdt.c -o kernel/gdt.o

kernel/idt.o: kernel/idt.c kernel/idt.h
	$(CC) $(CFLAGS) -c kernel/idt.c -o kernel/idt.o

OBJS = kernel/boot.o kernel/kernel.o kernel/vga.o \
       kernel/gdt.o kernel/gdt_flush.o \
       kernel/idt.o kernel/idt_flush.o \
       kernel/isr.o kernel/isr_stubs.o

pannos.bin: $(OBJS) kernel/linker.ld
	$(CC) -T kernel/linker.ld -o pannos.bin $(LDFLAGS) $(OBJS) -lgcc

pannos.iso: pannos.bin grub.cfg
	mkdir -p isodir/boot/grub
	cp pannos.bin isodir/boot/pannos.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o pannos.iso isodir

run: pannos.iso
	qemu-system-i386 -cdrom pannos.iso

clean:
	rm -rf kernel/*.o pannos.bin pannos.iso isodir

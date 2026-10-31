# PannOS

Toy OS kernel bare-metal for x86 (i686, 32 bit), written in C with minimum of assembly. The name comes from **Pannone**, which is the village I come from (200 inhabitants), with "OS". 

![PannOS in QEMU](assets/screenshot.png)

## Features (v0.1.0)

- Boot with **Multiboot2** + GRUB
- Driver **VGA** text mode (scroll, backspace)
- **GDT** flat
- **IDT**, exceptions CPU 0-31 with stub in assembly and handler in C
- **PIC 8259** remapped, 16 IRQ hardware with EOI
- **PIT 8253** a 100 Hz with a tick counter, uptime printed each second
- Board **PS/2** (IRQ1, scancode set 1, layout US, shift)
- Serial **port** COM1: everything that appears on the screen is also displayed there

An exception other than a breakpoint (`int 3`) is considered fatal: the kernel prints the number and halts, instead of re-executing the failed instruction infinitely.

## How to compile

You'll need a cross-compiler `i686-elf-gcc`. The script compiles from the source (binutils 2.41, gcc 13.2.0, it's a bit slow) and installs in `~/opt/cross`:

```bash
sudo apt install build-essential nasm xorriso grub-pc-bin qemu-system-x86 \
    libgmp-dev libmpfr-dev libmpc-dev texinfo
./setup-toolchain.sh
export PATH="$HOME/opt/cross/bin:$PATH"
make
```

## How to start

```bash
make run            # QEMU window
make run-headless   # without window, output on terminal via serial
```

You can also use the ISO from the release:

```bash
qemu-system-i386 -cdrom pannos.iso
```

## Limits

- Only 32 bit.
- Tried only on QEMU, non on real hardware.
- No memory managment, no processes, no user mode: today, the kernel runs, keeps time, and reads the keyboard

## Roadmap

5. Memory management: physical memory manager on memory map Multiboot2, then paging
6. Kernel heap (`kmalloc`/`kfree`)
7. Multitasking: context switch e scheduler round-robin
8. User mode e syscall (TSS, ring 3)
9. Minimum filesystem (initrd), loader ELF, shell in user mode
10. Stretch: x86_64, SMP, framebuffer VBE, net

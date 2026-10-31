#include <stdint.h>
#include "vga.h"
#include "serial.h"
#include "gdt.h"
#include "idt.h"
#include "pic.h"
#include "pit.h"

#define TICK_HZ 100

// stampa un numero decimale senza segno
static void print_uint(uint32_t n) {
    char buf[10];
    int i = 0;
    do {
        buf[i++] = '0' + n % 10;
        n /= 10;
    } while (n > 0);
    while (i > 0) {
        vga_putchar((unsigned char) buf[--i]);
    }
}

void kernel_main(void) {
    serial_init();
    vga_init();
    vga_write("PannOS v0.1.0\n");
    gdt_init();
    vga_write("[ok] gdt\n");
    idt_init();
    pic_remap(32, 40);
    vga_write("[ok] idt, pic\n");

    // solleva a mano l'eccezione 3, assembly inline perché in c non esiste
    // volatile o con -O2 la butta via, non avendo effetti visibili al compilatore
    // se si legge anche la riga dopo lo stub ha catturato e iret è tornato qui
    __asm__ volatile ("int $3");
    vga_write("[ok] eccezione 3 gestita\n");

    pit_init(TICK_HZ);
    pic_clear_mask(0); // timer
    pic_clear_mask(1); // tastiera
    __asm__ volatile ("sti");
    vga_write("[ok] pit 100 Hz, tastiera\n");
    vga_write("una riga di uptime al secondo, la tastiera scrive qui\n\n");

    uint32_t last = 0;
    for (;;) {
        uint32_t seconds = pit_read() / TICK_HZ;
        if (seconds != last) {
            last = seconds;
            vga_write("uptime ");
            print_uint(seconds);
            vga_write("s\n");
        }
        __asm__ volatile ("hlt");
    }
}

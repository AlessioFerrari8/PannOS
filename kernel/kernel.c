#include <stdint.h>
#include "vga.h"
#include "gdt.h"
#include "idt.h"
#include "pic.h"

void kernel_main(void) {
    vga_init();
    gdt_init();
    idt_init();
    pic_remap(32, 40);
    vga_write("prima\n");
    // solleva a mano l'eccezione 3, assembly inline perché in c non esiste
    // volatile o con -O2 la butta via, non avendo effetti visibili al compilatore
    // se si legge anche "dopo" lo stub ha catturato e iret è tornato qui
    __asm__ volatile ("int $3");
    pic_clear_mask(0);
    __asm__ volatile ("sti");
    vga_write("dopo\n");
    // 30 righe su 25 di schermo, così si vede se lo scroll funziona
    // for (int i = 0; i < 30; i++) {
    //     vga_putchar('A' + i);
    //     vga_write(" row\n");
    // }

    for (;;) {
        __asm__ volatile ("hlt");
    }
}

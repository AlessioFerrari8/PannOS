#include <stdint.h>
#include "vga.h"

void kernel_main(void) {
    gdt_init();
    vga_init();
    // 30 righe su 25 di schermo, così si vede se lo scroll funziona
    for (int i = 0; i < 30; i++) {
        vga_putchar('A' + i);
        vga_write(" riga\n");
    }
}

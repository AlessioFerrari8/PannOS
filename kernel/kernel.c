#include <stdint.h>
#include "vga.h"

void kernel_main(void) {
    vga_init();
    for (int i = 0; i < 30; i++) {
        vga_putchar('A' + i);
        vga_write(" riga\n");
    }
}

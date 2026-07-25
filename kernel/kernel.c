#include <stdint.h>
#include "vga.h"

void kernel_main(void) {
    vga_init();
    vga_write("Hi, my name is Alessio, and I'm a C dev \n");
}

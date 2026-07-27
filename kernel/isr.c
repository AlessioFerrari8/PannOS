#include <stdint.h>
#include "isr.h"
#include "vga.h"

void isr_handler(struct registers *regs) {
    vga_write("Exception raised: ");
    // mi calcolo decine e unita
    char ten = regs->int_no / 10;
    char units = regs->int_no % 10;
    vga_putchar('0' + (unsigned char) ten);
    vga_putchar('0' + (unsigned char)units);
    vga_write("\n");
}



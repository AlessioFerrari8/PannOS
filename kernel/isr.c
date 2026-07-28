#include <stdint.h>
#include "isr.h"
#include "vga.h"
#include "pic.h"
#define IRQ_BASE 32

// metodo che calcola decine e unità di un numero e le stampa con putchar
static void print_two_chars(int number) {
    char ten = number / 10;
    char units = number % 10;
    vga_putchar('0' + (unsigned char) ten);
    vga_putchar('0' + (unsigned char)units);
}

void isr_handler(struct registers *regs) {
    vga_write("ISR raised: ");
    // mi calcolo decine e unita
    print_two_chars(regs->int_no);
    vga_write("\n");
}

void irq_handler(struct registers *regs) {
    int irq_no = regs->int_no - IRQ_BASE;
    vga_write("IRQ raised: ");
    // mi calcolo decine e unita
    print_two_chars(irq_no);
    vga_write("\n");
    pic_send_eoi(irq_no);
}



#include <stdint.h>
#include "isr.h"
#include "vga.h"
#include "pic.h"
#include "pit.h"
#include "keyboard.h"
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

    // 3 = breakpoint, e' una trap: si riprende. Tutte le altre sono guasti, e con
    // iret la cpu rieseguirebbe la stessa istruzione all'infinito: fermo la macchina
    if (regs->int_no != 3) {
        vga_write("Eccezione fatale, sistema fermo\n");
        for (;;) {
            __asm__ volatile ("cli; hlt");
        }
    }
}

void irq_handler(struct registers *regs) {
    int irq_no = regs->int_no - IRQ_BASE;

    switch (irq_no) {
    case 0:
        pit_increase();
        break;
    case 1:
        keyboard_handle();
        break;
    default:
        vga_write("IRQ raised: ");
        print_two_chars(irq_no);
        vga_write("\n");
        break;
    }
    pic_send_eoi(irq_no);
}

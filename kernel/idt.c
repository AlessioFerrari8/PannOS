#include <stdint.h>
#include <stddef.h>
#include "idt.h"

// globali
static struct idt_entry idt[256]; // 256 perché la cpu x86 ha 256 vettori di interrupt
static struct idt_ptr idtp;


// helper
static void idt_set_entry(int num, uint32_t handler, uint16_t selector, uint8_t flags) {
    // 16 bit bassi dell'indirizzo dell'handler, li maschero con 0xFFFF
    idt[num].offset_low = handler & 0xFFFF;
    // bit 16-31, shifto a destra di 16 e maschero
    idt[num].offset_high = (handler >> 16) & 0xFFFF;
    // segmento in cui vive l'handler, 0x08 è il codice del kernel
    idt[num].selector = selector;
    // byte riservato, la cpu se lo aspetta a zero
    idt[num].zero = 0;
    // present, dpl e tipo di gate, 0x8E per eccezioni e irq
    idt[num].flags = flags;
}

void idt_init(void) {

    // riempio limit e base
    idtp.limit = sizeof(idt) - 1;
    idtp.base = (uint32_t)idt;

    // azzero tutte e 256 le entry
    for(size_t i = 0; i < sizeof(idt) / sizeof(idt[0]); i++) {
        idt_set_entry(i, 0, 0, 0);
    }

    idt_flush((uint32_t)&idtp);
}
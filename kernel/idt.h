#ifndef IDT_H
#define IDT_H
#include <stdint.h>

struct idt_entry {
    // 0-15 bit offset dell'handler
    uint16_t offset_low;
    // selettore del codice, 16 bit -> 0x08, entry 1 della gdt
    uint16_t selector;
    // byte sempre a zero, riservato
    uint8_t zero;
    // bit 7 present, 6-5 dpl, 4 sempre 0, 3-0 tipo di gate
    // 0x8E = present, ring 0, interrupt gate 32 bit
    uint8_t flags;
    // 16 bit alti dell'offset
    uint16_t offset_high;
}__attribute__((packed));

struct idt_ptr {
    // dimensione della tabella in byte - 1
    uint16_t limit;
    // indirizzo in memoria della prima entry
    uint32_t base;
}__attribute__((packed));


_Static_assert(sizeof(struct idt_entry) == 8, "idt_entry must be 8 bytes");
_Static_assert(sizeof(struct idt_ptr) == 6, "idt_ptr must be 6 bytes");

void idt_init(void);
void idt_flush(uint32_t);

#endif
#ifndef GDT_H
#define GDT_H
#include <stdint.h>

struct gdt_entry {
    // 0-15 del limite
    uint16_t limit_low;
    // 0-15 della base
    uint16_t base_low;
    // 16-23 della base
    uint8_t base_middle;
    // permessi + tipo
    uint8_t access;
    // bit 16-19 + 4 flag
    uint8_t granularity;
    // 24-31 della base
    uint8_t base_high;
}__attribute__((packed)); // packed, per evitare padding

struct gdt_ptr {
    // dimensione della tabella in byte - 1
    uint16_t limit;
    // indirizzo in memoria della prima entry
    uint32_t base;
}__attribute__((packed));

_Static_assert(sizeof(struct gdt_entry) == 8, "gdt_entry must be 8 bytes");
_Static_assert(sizeof(struct gdt_ptr) == 6, "gdt_ptr must be 6 bytes");


void gdt_init(void);
void gdt_flush(uint32_t);


#endif
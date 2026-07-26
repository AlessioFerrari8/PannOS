#include <stdint.h>
#include "gdt.h"

// globali
static struct gdt_entry gdt[3];
static struct gdt_ptr gp;

static void gdt_set_entry(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    // 16 bit bassi della base, li maschero con 0xFFFF
    gdt[num].base_low = base & 0xFFFF;
    // bit 16-23 shifto la base a destra di 16, maschero con 0xFF
    gdt[num].base_middle = (base >> 16) & 0xFF;
    // bit 24-31 shifto di 24, maschero con 0xFF
    gdt[num].base_high = (base >> 24) & 0xFF;
    // 16 bit bassi del limite
    gdt[num].limit_low = limit & 0xFFFF;

    gdt[num].access = access;
    gdt[num].granularity = ((limit >> 16) & 0xF) | (gran & 0xF0);
}

void gdt_init(void) {
    // 3 entry, null, cs per il codice, ds per i dati
    gdt_set_entry(0, 0, 0, 0, 0);
    gdt_set_entry(1, 0, 0xFFFFF, 0x9A, 0xC0);
    gdt_set_entry(2, 0, 0xFFFFF, 0x92, 0xC0);

    gp.limit = sizeof(gdt) - 1;
    gp.base = (uint32_t)gdt;

    // indirizzo di gp, castato per informare il compilatore
    gdt_flush((uint32_t) &gp);
}


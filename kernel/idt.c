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


// 32 stub
void isr0(void);
void isr1(void);
void isr2(void);
void isr3(void);
void isr4(void);
void isr5(void);
void isr6(void);
void isr7(void);
void isr8(void);
void isr9(void);
void isr10(void);
void isr11(void);
void isr12(void);
void isr13(void);
void isr14(void);
void isr15(void);
void isr16(void);
void isr17(void);
void isr18(void);
void isr19(void);
void isr20(void);
void isr21(void);
void isr22(void);
void isr23(void);
void isr24(void);
void isr25(void);
void isr26(void);
void isr27(void);
void isr28(void);
void isr29(void);
void isr30(void);
void isr31(void);

// indirizzi dei 32 stub, l'indice coincide col numero del vettore
static void (*const isr_stubs[32])(void) = {
    isr0,  isr1,  isr2,  isr3,  isr4,  isr5,  isr6,  isr7,
    isr8,  isr9,  isr10, isr11, isr12, isr13, isr14, isr15,
    isr16, isr17, isr18, isr19, isr20, isr21, isr22, isr23,
    isr24, isr25, isr26, isr27, isr28, isr29, isr30, isr31
};


void idt_init(void) {

    // riempio limit e base
    idtp.limit = sizeof(idt) - 1;
    idtp.base = (uint32_t)idt;

    // azzero tutte e 256 le entry
    for(size_t i = 0; i < sizeof(idt) / sizeof(idt[0]); i++) {
        idt_set_entry(i, 0, 0, 0);
    }

    // registro i 32 stub nelle prime 32 entry, sovrascrivendo gli zeri
    for(int i = 0; i < 32; i++) {
        idt_set_entry(i, (uint32_t)isr_stubs[i], 0x08, 0x8E);
    }

    idt_flush((uint32_t)&idtp);
}


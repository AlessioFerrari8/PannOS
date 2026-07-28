#include <stdint.h>
#include "pic.h"
#include "io.h"
// porta comando e porta dati dei due chip
#define MASTER_CMD  0x20
#define MASTER_DATA 0x21
#define SLAVE_CMD   0xA0
#define SLAVE_DATA  0xA1

#define ICW1_INIT 0x11
#define ICW4_8086 0x01
#define PIC_EOI   0x20

// all'accensione gli irq 0-7 arrivano sui vettori 8-15, dove ci sono già le
// eccezioni della cpu, quindi vanno spostati oltre il 31
void pic_remap(uint8_t offset1, uint8_t offset2) {
    // icw1 sulla porta comando, avvia la sequenza su entrambi
    outb(MASTER_CMD, ICW1_INIT);
    io_wait();
    outb(SLAVE_CMD, ICW1_INIT);
    io_wait();

    // icw2, il vettore di partenza di ciascuno
    outb(MASTER_DATA, offset1);
    io_wait();
    outb(SLAVE_DATA, offset2);
    io_wait();

    // icw3, come sono collegati: al master la bitmask della linea 2
    outb(MASTER_DATA, 0x04);
    io_wait();
    // allo slave il numero della linea, non la bitmask
    outb(SLAVE_DATA, 0x02);
    io_wait();

    // icw4, modalità 8086
    outb(MASTER_DATA, ICW4_8086);
    io_wait();
    outb(SLAVE_DATA, ICW4_8086);
    io_wait();

    // maschero tutto, sarà ogni driver a smascherarsi la sua linea
    outb(MASTER_DATA, 0xFF);
    outb(SLAVE_DATA, 0xFF);
}

void pic_set_mask(uint8_t irq) {
    uint16_t port;
    uint8_t mask;

    if (irq < 8) {
        port = MASTER_DATA;
    } else {
        port = SLAVE_DATA;
        // sullo slave l'irq 8 è il bit 0, il 15 è il bit 7
        irq -= 8;
    }

    mask = inb(port);
    // 1 = linea mascherata, accendo solo il bit che mi interessa
    mask = mask | (1 << irq);
    outb(port, mask);
}

void pic_clear_mask(uint8_t irq) {
    uint16_t port;
    uint8_t mask;

    if (irq < 8) {
        port = MASTER_DATA;
    } else {
        port = SLAVE_DATA;
        // gli irq dello slave passano per la linea 2 del master
        // se resta mascherata lì non arriva niente
        pic_clear_mask(2);
        irq -= 8;
    }

    mask = inb(port);
    // spengo il bit lasciando stare gli altri
    mask = mask & ~(1 << irq);
    outb(port, mask);
}

void pic_send_eoi(uint8_t irq) {
    if (irq >= 8) {
        outb(SLAVE_CMD, PIC_EOI);
    }
    outb(MASTER_CMD, PIC_EOI);
}
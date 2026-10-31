#include <stdint.h>
#include "io.h"
#include "serial.h"

#define COM1 0x3F8

void serial_init(void) {
    outb(COM1 + 1, 0x00); // niente interrupt dalla seriale
    outb(COM1 + 3, 0x80); // DLAB acceso: le porte 0/1 diventano il divisore
    outb(COM1 + 0, 0x01); // divisore 1 -> 115200 baud (byte basso)
    outb(COM1 + 1, 0x00); // byte alto
    outb(COM1 + 3, 0x03); // DLAB spento, 8 bit, nessuna parita', 1 stop
    outb(COM1 + 2, 0xC7); // FIFO attivo e svuotato, soglia 14 byte
    outb(COM1 + 4, 0x03); // DTR + RTS
}

void serial_putchar(char c) {
    // bit 5 della line status = holding register vuoto
    while (!(inb(COM1 + 5) & 0x20)) {
    }
    outb(COM1, (uint8_t) c);
}

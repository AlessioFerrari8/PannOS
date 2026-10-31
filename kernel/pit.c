#include <stdint.h>
#include "io.h"
#include "pit.h"
#define PIT_CMD 0x43
#define PIT_CH0 0x40
// il chip conta a questa frequenza fissa, l'unico modo di rallentarlo è dividerla
#define PIT_BASE 1193182

// scritto dall'handler e letto dal kernel: volatile
static volatile uint32_t ticks;
static uint32_t frequency;

void pit_init(uint32_t hz) {
    if (hz < 19) {
        hz = 19; // sotto i ~18.2 Hz il divisore non entra in 16 bit
    }
    frequency = hz;
    uint16_t divisor = PIT_BASE / hz;
    // canale 0, divisore in due byte, mode 2 (ricarica da solo), binario
    outb(PIT_CMD, 0x34);

    // il divisore è a 16 bit e la porta ne prende 8, prima la metà bassa poi l'alta
    outb(PIT_CH0, divisor & 0xFF);
    outb(PIT_CH0, divisor >> 8);
}

void pit_increase(void) {
    ticks++;
}

uint32_t pit_read(void) {
    return ticks;
}

uint32_t pit_hz(void) {
    return frequency;
}

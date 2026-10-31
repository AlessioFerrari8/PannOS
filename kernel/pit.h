#ifndef PIT_H
#define PIT_H
#include <stdint.h>

// programma il canale 0 alla frequenza richiesta (hz > 18, divisore a 16 bit)
void pit_init(uint32_t hz);
// chiamata dall'handler dell'IRQ0: un tick in piu'
void pit_increase(void);
// tick contati dall'avvio
uint32_t pit_read(void);
// frequenza impostata
uint32_t pit_hz(void);

#endif

#ifndef PIC_H
#define PIC_H
#include <stdint.h>

// sposta master e slave sui vettori indicati
void pic_remap(uint8_t offset1, uint8_t offset2);
// maschera una linea 0-15
void pic_set_mask(uint8_t irq);
// smaschera una linea
void pic_clear_mask(uint8_t irq);
// dice al PIC di aver finito
void pic_send_eoi(uint8_t irq);

#endif
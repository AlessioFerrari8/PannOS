#ifndef SERIAL_H
#define SERIAL_H

// programma COM1: 115200 baud, 8N1, FIFO attivo
void serial_init(void);
// manda un byte, aspettando che il trasmettitore sia libero
void serial_putchar(char c);

#endif

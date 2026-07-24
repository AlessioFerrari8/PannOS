#include <stdint.h>

unsigned char h = 'F';

void kernel_main(void) {
    unsigned char* p = (unsigned char*) 0xB8000;
    p[0] = h;
    p[1] = 0x0F;
}

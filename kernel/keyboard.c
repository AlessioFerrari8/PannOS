#include <stdint.h>
#include "io.h"
#include "keyboard.h"
#include "vga.h"

#define KBD_DATA 0x60

// scancode set 1 -> ascii, layout US. 0 = tasto senza carattere
static const char map[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' '
};
static const char map_shift[128] = {
    0, 27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
    0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,
    '*', 0, ' '
};

static int shift;

void keyboard_handle(void) {
    uint8_t code = inb(KBD_DATA);
    // bit 7 = tasto rilasciato (break code)
    int released = code & 0x80;
    code &= 0x7F;

    // 0x2A / 0x36 = shift sinistro / destro
    if (code == 0x2A || code == 0x36) {
        shift = !released;
        return;
    }
    if (released) {
        return;
    }
    char c = shift ? map_shift[code] : map[code];
    if (c) {
        vga_putchar((unsigned char) c);
    }
}

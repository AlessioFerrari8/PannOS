#include <stdint.h>
#include "vga.h"

// dichiarazioni delle funzioni, così vengono trovate subito
static void new_line(void);
static void vga_scroll(void);

// forzo a costanti, non devono essere cambiati
static const int height = 25;
static const int width = 80;
static int current_row = 0;
static int current_col = 0;
static int current_color;


static volatile uint16_t* buffer = (volatile uint16_t*) 0xB8000;

// helper
/**
 * @brief Compone il byte di attributo VGA a partire dai due colori
 * @param fg colore del testo, 0-15 (palette VGA standard)
 * @param bg colore di sfondo, 0-15; valori oltre 7 accendono il bit di blink
 * @return byte attributo -> bg nei 4 bit alti, fg nei 4 bassi
 */
static uint8_t compose_color(int fg, int bg) {
    // shifto bg di 4, cosi da fargli occupare i bit alti, e lo unisco a fg
    return bg << 4 | fg;
}

/**
 * @brief Compone la cella a 16 bit da scrivere nel buffer VGA
 * @param c carattere da mostrare (codepage 437, non ASCII puro)
 * @param atr byte attributo, tipicamente prodotto da compose_color()
 * @return cella pronta -> carattere nel byte basso, attributo nell'alto
 */
static uint16_t compose_cell(unsigned char c, uint8_t atr) {
    return c | atr << 8;
}

/**
 * @brief Inizializza il terminale: colore bianco su nero, cursore a (0,0)
 *        schermo ripulito. Va chiamata prima di qualsiasi altra vga_*.
 *
 * Per cancellare l'unico modo è riscrivere ogni cella
 */
void vga_init(void) {
    // current color, bianco su nero
    current_color = compose_color(15, 0);
    // azzero current_row e current_col
    current_row = current_col = 0;
    for(int r = 0; r < height; r++) {
        for(int c = 0; c < width; c++) {
            buffer[r * width + c] = compose_cell(' ', current_color);
        }
    }
}

/**
 * @brief Stampa un carattere alla posizione corrente e avanza il cursore
 * @param c carattere da stampare; '\n' manda a capo senza stampare nulla
 *
 * A fine riga va a capo da sola, a fine schermo fa scrollare. L'unica sequenza
 * di escape gestita è \n
 */
void vga_putchar(unsigned char c) {
    // caso speciale
    if (c == '\n') {
        new_line();
        return;
    } 
    
    // caso normale
    buffer[current_row * width + current_col] = compose_cell(c, current_color);
    current_col++;
    
    // se la colonna arriva alla fine
    if (current_col == width) {
        new_line();
    }
}

/**
 * @brief Stampa una stringa carattere per carattere
 * @param s stringa terminata da \0; il terminatore non viene stampato
 */
void vga_write(const char* s) {
    // indice
    int i = 0;

    // fino a quando non arriviamo alla fine della stringa
    while (s[i] != '\0') {
        vga_putchar((unsigned char) s[i]);
        i++;
    }
}

/**
 * @brief Fa scorrere lo schermo di una riga verso l'alto
 *
 * Il contenuto della riga 0 va perso definitivamente
 * L'ultima riga viene ripulita e il cursore resta su di essa
 */
static void vga_scroll(void) {
    // copia
    for(int r = 1; r < height; r++) { // parto da 1, per evitare -1
        for(int c = 0; c < width; c++) {
            buffer[(r-1) * width + c] = buffer[r * width + c];
        }
    }

    // pulizia ultima riga
    for(int c = 0; c < width; c++) {
        buffer[(height - 1) * width + c] = compose_cell(' ', current_color);
    }

    // cursore
    current_row = height - 1;
}

/**
 * @brief Porta il cursore all'inizio della riga successiva, scrollando se
 *        si era già sull'ultima riga
 */
static void new_line(void) {
    current_col = 0;
    current_row++;
    // se la colonna arriva alla fine
    if (current_row == height) {
        vga_scroll();
    }
}


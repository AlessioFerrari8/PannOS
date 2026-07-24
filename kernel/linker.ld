/* punto d'ingresso */
ENTRY(_start)

SECTIONS {
    /* inizio posizione 0x100000 */
    . = 1M;

    /* sezione text, codice eseguibile */
    .text : {
        /* prendi la sezione .text da tutti i file oggetto che il linker sta combinando */
        *(.multiboot)
        *(.text)
    }

    /* sezione rodata, dati costanti es. stringhe */
    .rodata : {
        *(.rodata)
    }

    /* sezione data, variabili globali inizializzate */
    .data : {
        *(.data)
    }

    /* sezione bss, variabili globali non inizializzate */
    .bss : {
        *(.bss)
    }
}

/* punto d'ingresso */
ENTRY(_start)

SECTIONS {
    /* inizio a 0x100000, sotto il mega c'è roba di bios e hardware legacy */
    . = 1M;

    /* sezione text, codice eseguibile */
    .text : {
        /* .multiboot va per prima, grub cerca l'header solo nei primi 32 kb */
        *(.multiboot)
        /* prende la .text da tutti i file oggetto che il linker sta combinando */
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

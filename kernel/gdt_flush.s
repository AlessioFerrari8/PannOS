; gdt_flush - installa la GDT e ricarica i segment register
; dal C: void gdt_flush(uint32_t gdt_ptr_addr)
; serve assembly perché lgdt e il far jump non ci sono in C

section .text
global gdt_flush

gdt_flush:
    ; cdecl: parametri sullo stack, in [esp] c'è l'indirizzo di ritorno
    ; quindi il primo parametro sta 4 byte più avanti
    mov eax, [esp+4]

    ; legge i 6 byte all'indirizzo in eax e li copia nel GDTR
    ; ora la CPU sa dov'è la tabella, ma i segment register sono ancora di GRUB
    lgdt [eax]

    ; 0x10 = selettore dati, cioè la entry 2
    ; passo per ax perché non esiste mov da immediato a segment register
    mov ax, 0x10

    ; riscriverli è l'unico modo per fargli rileggere il descrittore nuovo
    ; anche fs e gs che non uso, altrimenti restano puntati alla GDT di GRUB
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; mov cs, ax non esiste: cs ed eip vanno cambiati insieme
    ; il far jump è l'unica istruzione che lo fa
    ; .flush è la riga qui sotto, conta solo l'effetto collaterale su cs
    jmp 0x08:.flush

.flush:
    ; torna a gdt_init
    ret

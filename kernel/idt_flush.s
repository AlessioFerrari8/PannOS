section .text
global idt_flush

idt_flush:
    ; cdecl: parametri sullo stack, in [esp] c'è l'indirizzo di ritorno
    ; quindi il primo parametro sta 4 byte più avanti
    mov eax, [esp+4]

    ; carico la tabella
    lidt [eax]

    ; torna a idt_init
    ret
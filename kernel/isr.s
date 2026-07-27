; isr.s - stub di ingresso per le eccezioni 0-31
; uno per vettore perché la cpu non dice quale eccezione è scattata
; l'unico indizio è da quale entry della idt è saltata, quindi il numero
; lo cabla lo stub e poi confluiscono tutti in isr_common_stub

section .text

; eccezioni che non pushano l'error code
; lo zero finto non serve a niente se non a occupare lo spazio
; così il frame che arriva a isr_common_stub ha sempre la stessa forma
%macro ISR_NOERRCODE 1
    global isr%1
    isr%1:
        push 0
        push %1

        jmp isr_common_stub
%endmacro

; qui niente zero finto, lo spazio l'ha già occupato la cpu
%macro ISR_ERRCODE 1
    global isr%1
    isr%1:
        push %1

        jmp isr_common_stub
%endmacro

; 8 10 11 12 13 14 17 sono le uniche con error code
ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE 8
ISR_NOERRCODE 9
ISR_ERRCODE 10
ISR_ERRCODE 11
ISR_ERRCODE 12
ISR_ERRCODE 13
ISR_ERRCODE 14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_ERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31



; punto di arrivo comune, per ora non fa niente e torna indietro
isr_common_stub:
    add esp, 8

    ; ret non basterebbe, non ripristina né cs né eflags
    ; e senza eflags if resterebbe spento per sempre
    iret
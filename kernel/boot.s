; sezione con header multiboot
section .multiboot
; indirizzo di memoria del primo byte dell'header 
multiboot_start:
; magic 
dd 0xE85250D6
; architettura
dd 0
dd multiboot_end - multiboot_start
; magic + architecture + header_length, tutto negato
; il risultato deve fare 0, così si verifica che l'header non sia corrotto
dd -(0xE85250D6 + 0 + (multiboot_end - multiboot_start))

; l'header finisce qui
dd 0    ; tipo
dd 0    ; flags
dd 8    ; size del tag stesso

multiboot_end:

; stack (inverto top e bottom essendo una struttura che cresce verso il basso)
section .bss
align 16 ; l'abi x86 vuole lo stack allineato a 16 byte alle chiamate
stack_bottom:
resb 16384 ; 16 kb, abbondanti finché non ci sono interrupt annidati
stack_top:

; _start
section .text
; rende l'etichetta _start visibile al linker (ENTRY(_start))
global _start
extern kernel_main

_start:
    mov esp, stack_top ; punto esp in cima allo stack riservato prima
    call kernel_main ; chiamo la funzione C 

    cli ; disattivo tutti gli interrupt
.hang:
    hlt ; fermo la cpu
    jmp .hang ; se un interrupt la risveglia comunque, mi fermo di nuovo
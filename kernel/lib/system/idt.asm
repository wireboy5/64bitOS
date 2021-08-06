[bits 64]
[extern isr_handler]
%include "kernel/utility/asm/macros.s"

[global register_interrupt_handlers]
[global load_idt]

%macro ISR_NOERRCODE 1
    [GLOBAL isr%1]
    isr%1:
        cli
        push 0
        push %1
        jmp isr_common_stub
%endmacro

%macro ISR_ERRCODE 1
    [GLOBAL isr%1]
    isr%1:
        cli
        push %1
        jmp isr_common_stub
%endmacro

isr_common_stub:
    ; Push all general purpose registers
    pushAll
    
    ; Save data segment descriptor
    mov ax, ds
    push rax


    mov ax, 0x0 ; Load kernel segment descriptors
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Call the isr handler
    call isr_handler

    ; Reload original segment descriptors
    pop rax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    
    ; Pop all general purpose registers
    popAll

    iretq




; ISRs
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
ISR_ERRCODE 21

; Now we want the rest of the ISRs to be defined as NOERRCODE
%assign i 22
%rep 234
    ISR_NOERRCODE i
%assign i i+1
%endrep

register_interrupt_handler:
    ; Registers interrupt handler pointed to by rsi to entry number in rdi
    ; selector rdx, and flags rcx


    ; Base low (first 16 bits)
    mov [rdi], si

    ; Selector
    mov [rdi + 2], dx

    ; Flags
    mov [rdi + 5], cl

    ; Base mid (second 16 bits)
    shr rsi, 16
    mov [rdi + 6], si
    ; Base high (third 32 bits)
    shr rsi, 16
    mov [rdi + 8], esi
    



    ret

register_interrupt_handlers:
    ; Registers all interrupt handlers.
    ; rdi contains the location of the table

    mov rdx, 0x0
    mov rcx, 0x8E

%assign i 0 
%rep    256
    mov rsi, isr%[i]
    ; Register interrupt handler
    call register_interrupt_handler
    ; Add offset to entry number
    add rdi, 16
%assign i i+1
%endrep
    ret

load_idt:
    ; Loads the IDT
    ; rdi contains the location of the IDT
    lidt [rdi]
    ret


section .bss:

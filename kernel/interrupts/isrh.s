%include "kernel/utility/asm/macros.s"

extern isr_handler

isr_common:

    ; This push is for the function to consume
    pushAll

    ; Call the handler
    call isr_handler

    ; Pop what the function consumed
    popAll

    popAll ; Pop all registers that were pushes before this common
    ; label was jumped to

    ; Cleanup error code and isr number
    add esp, 16

    iretq ; Return (long mode interrupt)



global idt_flush

idt_flush:
    mov rax, [rsp+8]
    lidt [rax]

    ret

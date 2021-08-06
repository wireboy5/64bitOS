#include "interrupts.h"

idt_entry_t idt[256];
idt_pointer_t idt_ptr;

void init_idt() {
    idt_ptr.limit = (sizeof(idt_entry_t) * 256) - 1;
    idt_ptr.base = (uint64_t)&idt - 0xffffffff80000000;
    

    idt_pointer_t* ipt = (idt_pointer_t*)((&idt_ptr));
    char c[33];
    itoa(ipt, c, 16);
    serial_print(c);
    serial_print("\n");
    register_interrupt_handlers(&idt);

    load_idt(&idt_ptr - 0xffffffff80000000);
}

void isr_handler(isr_regs_t *regs) {
    char c[33];
    itoa(regs->int_no, c, 16);
    serial_print("Interrupt:");
    serial_print(c);
    serial_print("\n");
}
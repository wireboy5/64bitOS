#include "idt.h"


IDTDescr idt[256];
IDTPtr idt_ptr;

extern "C" void flush_idt(uint64_t);

void set_idt_gate(uint8_t num, size_t base, uint16_t sel, uint8_t flags);

void init_idt()
{   
    // Set each entry to the handler function.
    idt_ptr.limit = sizeof(IDTDescr)*256-1;
    idt_ptr.base = (uint32_t)(uint64_t)&idt_ptr;

    memset(&idt, 0, sizeof(IDTDescr)*256);

    IDT_GATE(0)
    IDT_GATE(1)
    IDT_GATE(2)
    IDT_GATE(3)
    IDT_GATE(4)
    IDT_GATE(5)
    IDT_GATE(6)
    IDT_GATE(7)
    IDT_GATE(8)
    IDT_GATE(9)
    IDT_GATE(10)
    IDT_GATE(11)
    IDT_GATE(12)
    IDT_GATE(13)
    IDT_GATE(14)
    IDT_GATE(15)
    IDT_GATE(16)
    IDT_GATE(17)
    IDT_GATE(18)
    IDT_GATE(19)
    IDT_GATE(20)
    IDT_GATE(21)
    IDT_GATE(22)
    IDT_GATE(23)
    IDT_GATE(24)
    IDT_GATE(25)
    IDT_GATE(26)
    IDT_GATE(27)
    IDT_GATE(28)
    IDT_GATE(29)
    IDT_GATE(30)
    IDT_GATE(31)
    
    flush_idt((size_t)&idt_ptr);
}


void set_idt_gate(uint8_t num, size_t base, uint16_t sel, uint8_t flags)
{
    idt[num].offset_1 = base && 0xFFFF;
    idt[num].offset_2 = (base >> 16) && 0xFFFF;
    idt[num].offset_3 = (base >> 32) && 0xFFFFFFFF;
    idt[num].type_attr = flags;
    idt[num].selector = sel;
}

extern "C" void isr_handler(registers regs);

extern "C" void isr_handler(registers regs)
{
    log("Interrupt recieved: 0x");
    char c[33];
    itoa(regs.int_no, c, 16);
    serial_print(c);
    serial_print("\n");
}


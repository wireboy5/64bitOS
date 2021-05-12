#pragma once

#include <stdint.h>
#include <stddef.h>

#include <kernel/lib/string.h>
#include <kernel/lib/logging.h>
#include <kernel/lib/serial.h>

void init_idt();


struct IDTDescr {
   uint16_t offset_1; // offset bits 0..15
   uint16_t selector; // a code segment selector in GDT or LDT
   uint8_t ist;       // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
   uint8_t type_attr; // type and attributes
   uint16_t offset_2; // offset bits 16..31
   uint32_t offset_3; // offset bits 32..63
   uint32_t zero;     // reserved
} __attribute__((packed));

struct IDTPtr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

struct registers
{
    uint64_t ds;
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8, rdi, rsi, rbp, useless, rbx, rdx, rcx, rax;
    uint64_t int_no, err_code;
    uint64_t rip, cs, eflags, rsp, ss;
} __attribute__((packed));

#define IDT(i) extern "C" void isr##i();
#define IDT_GATE(i) set_idt_gate(i,(size_t)isr##i, 0x08, 0x8E);

IDT(0)
IDT(1)
IDT(2)
IDT(3)
IDT(4)
IDT(5)
IDT(6)
IDT(7)
IDT(8)
IDT(9)
IDT(10)
IDT(11)
IDT(12)
IDT(13)
IDT(14)
IDT(15)
IDT(16)
IDT(17)
IDT(18)
IDT(19)
IDT(20)
IDT(21)
IDT(22)
IDT(23)
IDT(24)
IDT(25)
IDT(26)
IDT(27)
IDT(28)
IDT(29)
IDT(30)
IDT(31)

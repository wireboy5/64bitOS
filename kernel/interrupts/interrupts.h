#pragma once
#include <stdint.h>


#include <io/serial.h>

typedef struct registers {
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
    uint64_t rdi, rsi, rbp, rbx, rdx, rcx, rax;
    uint8_t int_no, err_code;
    uint64_t rip, cs, eflags, rsp, ss;
} registers_t;

extern void isr_handler(registers_t regs);
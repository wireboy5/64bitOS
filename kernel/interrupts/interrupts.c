#include "interrupts.h"

extern void isr_handler(registers_t regs) {
    serial_print("Interrupt Received");
}
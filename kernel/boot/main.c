#include "main.h"


extern void kmain() {
    // Initialize serial
    init_serial();
    serial_print("Serial Initialized\n");

    // Initialize interrupts
    initialize_idt();
    serial_print("IDT Initialized\n");

    asm volatile ("int $0x3");
}
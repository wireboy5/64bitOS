#include "main.h"


extern void kmain(void* multiboot_info) {
    
    // Initialize serial
    init_serial();

    // Print extra newline on serial
    serial_print("\n");

    // Print booting message
    serial_print("Kernel Loa")
}
#include "main.h"


extern void kmain(void* multiboot_info) {
    
    // Initialize serial
    init_serial();

    // Print extra newline on serial
    serial_print("\n");

    // Load system info
    sysinfo_t info = get_sysinfo(multiboot_info);
}
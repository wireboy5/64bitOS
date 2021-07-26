#include "main.h"

extern size_t _kernel_end;

extern void kmain(void* multiboot_info) {
    
    // Initialize serial
    init_serial();

    // Print extra newline on serial
    serial_print("\n");

    // Load system info
    sysinfo_t info = get_sysinfo(multiboot_info);

    char c[33];
    itoa(&multiboot_info, c, 16);
    serial_print(c);
    serial_print("\n");
    
    itoa(&_kernel_end, c, 16);
    serial_print(c);
    serial_print("\n");
}
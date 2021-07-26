#include "main.h"

extern size_t _kernel_end_phys;

extern void kmain(void* multiboot_info) {
    
    // Initialize serial
    init_serial();

    // Print extra newline on serial
    serial_print("\n");

    // Load system info
    sysinfo_t info = get_sysinfo(multiboot_info);

    char c[33];
    itoa(info.fb.address >> 32, c, 16);
    serial_print(c);
    itoa(info.fb.address, c, 16);
    serial_print(c);
    serial_print("\n");
    
    itoa(&_kernel_end_phys, c, 16);
    serial_print(c);
    serial_print("\n");
}
#include "main.h"


extern void kmain(void* multiboot_info) {
    
    // Initialize serial
    init_serial();

    // Print extra newline on serial
    serial_print("\n");

    // Load system info
    sysinfo_t info = get_sysinfo(multiboot_info);

    // Test frame buffer
    for(uint32_t y = 0; y < info.fb.height; y++) {
        for(uint32_t x = 0; x < info.fb.width; x++) {
            set_pixel(info, x, y, 0xFFFFFFFF);
        }
    }
}
#include "main.h"


extern void kmain(void* multiboot_info) {
    
    // Initialize serial
    init_serial();

    // Print extra newline on serial
    serial_print("\n");

    // Load system info
    sysinfo_t info = get_sysinfo(multiboot_info);

    // Test frame buffer
    for(int y = 0; y < info.fb.height; y++) {
        for(int x = 0; x < info.fb.width; x++) {
            set_pixel(info, x, y, 0xFFFFFFFF);
        }
    }
}
#include <kernel/main.h>

// Kernel offset, start, and end
extern "C" size_t kernelOffset;
extern "C" size_t _kernel_start;

// Multiboot 2 info
extern "C" size_t mboot_info;

extern "C" int kmain () {

    
    clear_screen();
    
    return 0;
}
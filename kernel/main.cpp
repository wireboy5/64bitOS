#include <kernel/main.h>

// Kernel offset, start, and end
extern "C" size_t kernelOffset;
extern "C" size_t _kernel_start;



extern "C" int kmain () {

    // Test that higher half works
    char* vidmem_high = (char*)(0xC00B8000);
    vidmem_high[2] = 'H';

    
    return 0;
}
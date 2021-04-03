#include <kernel/main.h>

// Kernel offset, start, and end
extern "C" size_t kernelOffset;
extern "C" size_t _kernel_start;

extern "C" int kmain () {
    // Test that lower 2MB works
    char* vidmem = (char*)(0xb8000);
    vidmem[0] = 'L';

    // Test that higher half works
    char* vidmem_high = (char*)(0xb8000 + 0xC0000000);
    vidmem[2] = 'H';

    return 0;
}
#include <kernel/main.h>

// Kernel offset, start, and end
extern "C" size_t kernelOffset;
extern "C" size_t _kernel_start;

extern "C" int kmain () {
    char* vidmem = (char*)0xb8000;
    vidmem[0] = 'C';
    return 0;
}
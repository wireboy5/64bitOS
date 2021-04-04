#include <kernel/main.h>

// Kernel offset, start, and end
extern "C" size_t kernelOffset;
extern "C" size_t _kernel_start;

void kprint(){

}

extern "C" int kmain () {

    // Test that higher half works
    char* vidmem_high = (char*)(0xb8000 + 0xC0000000);
    vidmem_high[2] = 'H';

    kprint();
    return 0;
}
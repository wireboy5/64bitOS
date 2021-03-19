#include <kernel/main.h>

// Kernel offset, start, and end
extern "C" size_t kernelOffset;
extern "C" size_t _kernel_start;

extern "C" int kmain () {
    // Lets go paging!
    return 0;
}
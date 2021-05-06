#include <kernel/main.h>

// Kernel offset, start, and end
extern "C" size_t kernelOffset;
extern "C" size_t _kernel_start;

// Multiboot 2 info
extern "C" void* mboot_info;

char* fb_addr;

extern "C" int kmain () {

    sysinfo info = get_info(mboot_info);

    sprint("test");

    return 0;
}
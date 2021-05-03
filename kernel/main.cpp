#include <kernel/main.h>

// Kernel offset, start, and end
extern "C" size_t kernelOffset;
extern "C" size_t _kernel_start;

// Multiboot 2 info
extern "C" size_t mboot_info;

extern "C" int kmain () {

    
    clear_screen();
    put_char('a',0xf,0x0);
    put_char('b',0xf,0x0);
    put_char('\n',0xf,0x0);
    put_char('c',0xf,0x0);
    return 0;
}
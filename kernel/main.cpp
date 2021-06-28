#include <kernel/main.h>

extern "C" size_t kernOffset;



extern "C" int kmain (void* mboot) {
    log("\nKernel Successfully Bootstrapped\n");

    sysinfo info = get_info(mboot);
    
    
    // We need interrupts as soon as possible


    char c[33];
    itoa(info.fb.addr, c, 16);
    log("Found framebuffer at",1);
    serial_print(c);
    serial_print("\n");
    
    return 0;
}
#include <kernel/main.h>

extern "C" size_t kernOffset;



extern "C" int kmain (void* mboot) {
    log("\nhello,world!\n");

    sysinfo info = get_info(mboot);
    
    char c[33];
    itoa(info.fb.height, c, 10);
    log(c);
    serial_print("\n");

    init_idt();
    
    asm("int $3");
    
    return 0;
}
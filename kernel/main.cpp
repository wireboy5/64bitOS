#include <kernel/main.h>



// Multiboot 2 info
extern "C" void* mboot_info;


extern "C" int kmain () {
    sprint("\nhello,world!\n");
    return 0;
}
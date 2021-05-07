#include <kernel/main.h>



// Multiboot 2 info
extern "C" void* mboot_info;


extern "C" int kmain () {
    for(;;);
    return 0;
}
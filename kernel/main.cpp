#include <kernel/main.h>



// Multiboot 2 info
extern "C" void* mboot_info;


extern "C" int kmain () {
    char c[] = "\nhello,world!\n";
    sprint(c);
    return 0;
}
#include <kernel/main.h>





extern "C" int kmain (void* mboot) {
    sprint("\nhello,world!\n");

    sysinfo info = get_info(mboot);
    
    

    
    
    return 0;
}
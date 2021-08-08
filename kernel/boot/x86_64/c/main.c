#include "main.h"

extern size_t _kernel_end_phys;

extern void kmain(void* multiboot_info) {
    
    // Initialize serial
    init_serial();

    // Log kernel start loading
    log(1, "Kernel loading.\n");

    // Load system info
    system_info = get_sysinfo(multiboot_info);

    
}
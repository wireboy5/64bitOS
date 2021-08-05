#include "main.h"

extern size_t _kernel_end_phys;

extern void kmain(void* multiboot_info) {
    
    // Initialize serial
    init_serial();

    // Print extra newline on serial
    serial_print("\n");

    // Load system info
    system_info = get_sysinfo(multiboot_info);

    // Initialize the page frame allocator
    init_pfa(&system_info);

    // Sort the memory map again
    sort_memmap(system_info.memmap);
    
    
    

     // Print memory map
    for(uint32_t i = 0; i < system_info.memmap->index; i++) {
        memmap_entry_t entry = system_info.memmap->entries[i];
        serial_print("OS MM Entry: ");
        char c[33];

        itoa(entry.start, c, 16);
        serial_print(c);
        serial_print(" - ");
        uint64_t end_addr = (uint64_t)entry.start + (uint64_t)entry.size;
        
        itoa(end_addr, c, 16);
        serial_print(c);

        serial_print(" ( Size: ");
        itoa(entry.size, c, 16);
        serial_print(c);
        serial_print(" )");
        
        if (entry.flags == MEMMAP_ENTRY_AVAILABLE) {
            serial_print(" - Available");
        } else if (entry.flags == MEMMAP_ENTRY_RESERVED) {
            serial_print(" - Reserved");
        } else if (entry.flags == MEMMAP_ENTRY_MULTIBOOT_INFO) {
            serial_print(" - Multiboot Info");
        } else if (entry.flags == MEMMAP_ENTRY_MODULE) {
            serial_print(" - Module");
        } else if (entry.flags == MEMMAP_ENTRY_KERNEL) {
            serial_print(" - Kernel");
        } else if (entry.flags == MEMMAP_ENTRY_BAD_RAM) {
            serial_print(" - Bad Ram");
        } else if (entry.flags == MEMMAP_ENTRY_ACPI_NVS) {
            serial_print(" - ACPI NVS");
        } else if (entry.flags == MEMMAP_ENTRY_ACPI_RECLAIMABLE) {
            serial_print(" - ACPI Reclaimable");
        } else if (entry.flags == MEMMAP_ENTRY_FRAMEBUFFER) {
            serial_print(" - Framebuffer");
        } else if (entry.flags == MEMMAP_ENTRY_PFA_BITMAP) {
            serial_print(" - PFA Bitmap");
        } else {
            serial_print(" - Unknown");
        }
        serial_print("\n");
    }

    // Print the bitmap for the PFA
    print_bitmap(system_info);


    // Initialize paging system
    init_paging(&system_info);
}
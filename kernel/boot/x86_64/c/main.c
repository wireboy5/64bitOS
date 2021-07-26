#include "main.h"

extern size_t _kernel_end_phys;

extern void kmain(void* multiboot_info) {
    
    // Initialize serial
    init_serial();

    // Print extra newline on serial
    serial_print("\n");

    // Load system info
    sysinfo_t info = get_sysinfo(multiboot_info);

    // Print memory map

    
    
    
    for (struct multiboot_mmap_entry* entry = (struct multiboot_mmap_entry*) ((uintptr_t)info.mmap + sizeof(struct multiboot_tag_mmap));
        (uintptr_t)entry - (uintptr_t)info.mmap < info.mmap->size; entry = (struct multiboot_mmap_entry*)((uintptr_t)entry + info.mmap->entry_size)) {
        char c[33];
        itoa((uintptr_t)entry, c, 16);
        serial_print(c);

        serial_print("MMAP Entry: ");

        itoa(entry->addr >> 32, c, 16);
        serial_print(c);
        itoa(entry->addr, c, 16);
        serial_print(c);
        serial_print(" - ");
        uint64_t end_addr = entry->addr + entry->len;
        itoa(end_addr >> 32, c, 16);
        serial_print(c);
        itoa(end_addr, c, 16);
        serial_print(c);
        
        // Switch on type
        switch (entry->type) {
            case MULTIBOOT_MEMORY_AVAILABLE:
                serial_print(" - Available\n");
                break;
            case MULTIBOOT_MEMORY_RESERVED:
                serial_print(" - Reserved\n");
                break;
            case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
                serial_print(" - ACPI reclaimable\n");
                break;
            case MULTIBOOT_MEMORY_NVS:
                serial_print(" - ACPI NVS\n");
                break;
            case MULTIBOOT_MEMORY_BADRAM:
                serial_print(" - Bad ram\n");
                break;
            default:
                serial_print(" - Unknown\n");
        }
    }
}
#include "memmap.h"


extern char* _kernel_start_phys;
extern char* _kernel_end_phys;

/*  INTERNAL FUNCTION
    Shifts all zero-size entries to the end of the map, adjusts the index, and then sorts the memory map
    by start address. All "free" entries are then page aligned up the the nearest page boundary.
    Any overlapping tables are truncated according to precedence.
*/
void memmap_cleanup_internal(memmap_t* memmap, bool round, bool truncate);


/*
    Generates a memory map based off of system information.
*/
memmap_t generate_memmap(sysinfo_t sysinfo) {
    memmap_t memmap;

    memmap.index = 0;
    
    for(
        struct multiboot_mmap_entry* entry = (struct multiboot_mmap_entry*)((uint64_t)sysinfo.mboot_mmap + sizeof(struct multiboot_tag_mmap));
        (uint64_t)entry - (uint64_t)sysinfo.mboot_mmap < sysinfo.mboot_mmap->size;
        entry = (struct multiboot_mmap_entry*)((uint64_t)entry + (uint64_t)sysinfo.mboot_mmap->entry_size)
    ){
        uint8_t type;

        switch (entry->type) {
            case MULTIBOOT_MEMORY_AVAILABLE:
                type = memmap_type_free;
                break;
            case MULTIBOOT_MEMORY_RESERVED:
                type = memmap_type_reserved;
                break;
            case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
                type = memmap_type_acpi_reclaimable;
                break;
            case MULTIBOOT_MEMORY_NVS:
                type = memmap_type_acpi_nvs;
                break;
            case MULTIBOOT_MEMORY_BADRAM:
                type = memmap_type_bad_ram;
                break;
            default:
                type = memmap_type_unknown;
        }

        // Set flags
        uint8_t flags = 0b00000001;

        // Set start
        uint64_t start = entry->addr;

        // Set end
        uint64_t end = entry->addr + entry->len;

        

        // Create new memmap entry
        memmap_entry_t new_entry = { start, end, type, flags };
        memmap.entries[memmap.index] = new_entry;
        memmap.index++;

        log(0, "MMAP Entry: %d: %x - %x\n", new_entry.type, new_entry.start, new_entry.end);
    }

    // Add kernel to memmap
    memmap_entry_t new_entry = { (uint64_t)&_kernel_start_phys, (uint64_t)&_kernel_end_phys, memmap_type_kernel_loaded, 0b10 };
    memmap.entries[memmap.index] = new_entry;
    memmap.index++;

    // Cleanup memmap
    memmap_cleanup_internal(&memmap, true, true);

    // Return memory map
    return memmap;
}

/*
    Shifts all zero-size entries to the end of the map, adjusts the index, and then sorts the memory map
    by start address. All "free" entries are then page aligned up the the nearest page boundary.
    Any overlapping tables are truncated according to precedence.
*/

void memmap_cleanup(memmap_t* memmap) {
    memmap_cleanup_internal(memmap, false, false);
}

/*  INTERNAL FUNCTION
    Shifts all zero-size entries to the end of the map, adjusts the index, and then sorts the memory map
    by start address. All "free" entries are then page aligned up the the nearest page boundary.
    Any overlapping tables are truncated according to precedence.
*/
void memmap_cleanup_internal(memmap_t* memmap, bool round, bool truncate) {
    
    // Shift all zero-size entries to the end of the map
    bool shifted = true;
    while (shifted) {
        shifted = false;
        for (uint64_t i = 0; i < memmap->index; i++) {
            if (memmap->entries[i].end <= memmap->entries[i].start) {
                // Swap with next entry
                memmap_entry_t temp = memmap->entries[i];
                memmap->entries[i] = memmap->entries[i + 1];
                memmap->entries[i + 1] = temp;
                shifted = true;
            }
        }
    }

    // Adjust index
    for(uint64_t i = 0; i < memmap->index; i++) {
        if(memmap->entries[i].end == memmap->entries[i].start) {
            memmap->index = i;
            break;
        }
    }

    // Sort by start address
    bool sorted = true;
    while (sorted) {
        sorted = false;
        for (uint64_t i = 0; i < memmap->index - 1; i++) {
            if (memmap->entries[i].start > memmap->entries[i + 1].start) {
                // Swap with next entry
                memmap_entry_t temp = memmap->entries[i];
                memmap->entries[i] = memmap->entries[i + 1];
                memmap->entries[i + 1] = temp;
                sorted = true;
            }
        }
    }


    


    // Now we need to truncate overlapping entries.
    // Specifically, if they are provide by multiboot, they will be the ones being truncated.
    if(truncate) {
        uint64_t index = memmap->index;
        // Truncate all overlapping entries
        for (uint64_t i = 0; i < index; i++) {
            // If this entry is created by the kernel
            if (!(memmap->entries[i].flags & memmap_flag_multiboot)) {
                // Check if entry to the left overlapps
                if (i > 0 && memmap->entries[i - 1].end >= memmap->entries[i].start && i != 0) {
                    // If it does, split it into two entries
                    memmap_entry_t new_entry = { memmap->entries[i - 1].start, memmap->entries[i].start, memmap->entries[i - 1].type, memmap->entries[i - 1].flags };
                    
                    memmap_entry_t new_entry2 = { memmap->entries[i].end, memmap->entries[i - 1].end, memmap->entries[i - 1].type, memmap->entries[i - 1].flags };

                    // Insert new entries
                    memmap->entries[memmap->index] = new_entry;
                    memmap->entries[memmap->index + 1] = new_entry2;
                    memmap->index += 2;

                    // Remove old entry
                    memmap->entries[i - 1].end = memmap->entries[i - 1].start;
                
                    // If they are invalid, they will be cleaned up later
                }

                // Check if entry to the right overlapps
                if (i < memmap->index - 1 && memmap->entries[i + 1].start <= memmap->entries[i].end && i != index) {
                    // If it does, split it into two entries
                    memmap_entry_t new_entry = { memmap->entries[i + 1].start, memmap->entries[i].start, memmap->entries[i + 1].type, memmap->entries[i + 1].flags };

                    memmap_entry_t new_entry2 = { memmap->entries[i].end, memmap->entries[i + 1].end, memmap->entries[i + 1].type, memmap->entries[i + 1].flags };

                    // Insert new entries
                    memmap->entries[memmap->index] = new_entry;
                    memmap->entries[memmap->index + 1] = new_entry2;
                    memmap->index += 2;

                    // Remove old entry
                    memmap->entries[i + 1].end = memmap->entries[i + 1].start;

                    // If they are invalid, they will be cleaned up later
                }
            }
        }
    }

    if(round) {
        // Round up all free entries to the nearest page boundary
        for (uint64_t i = 0; i < memmap->index; i++) {
            uint64_t old_start = memmap->entries[i].start;
            uint64_t old_end = memmap->entries[i].end;
            // If this is free
            if (memmap->entries[i].type == memmap_type_free) {
                // Check if start is page aligned
                if ((memmap->entries[i].start & ~0xFFFULL) != 0) {
                    // Round to next page
                    memmap->entries[i].start &= ~0xFFFULL;
                    memmap->entries[i].start += 0x1000;

                    // Set flag
                    memmap->entries[i].flags |= memmap_flag_page_rounded_up;
                }

                // Check if end is page aligned
                if ((memmap->entries[i].end & ~0xFFFULL) != 0) {
                    // Round to previous page
                    memmap->entries[i].end &= ~0xFFFULL;

                    // Set flag
                    memmap->entries[i].flags |= memmap_flag_page_rounded_down;
                }

                // If the end is less than the start
                if (memmap->entries[i].end < memmap->entries[i].start) {
                    // Set as reserved
                    memmap->entries[i].type = memmap_type_reserved;

                    // Reset start and end
                    memmap->entries[i].start = old_start;
                    memmap->entries[i].end = old_end;

                    // Set flag
                    memmap->entries[i].flags |= memmap_flag_unable_to_round;
                }
            }
        }
    }

    
    // Repeat these steps again. only if truncated

    if(truncate) {
        // Shift all zero-size entries to the end of the map
        shifted = true;
        while (shifted) {
            shifted = false;
            for (uint64_t i = 0; i < memmap->index; i++) {
                if (memmap->entries[i].end <= memmap->entries[i].start) {
                    // Swap with next entry
                    memmap_entry_t temp = memmap->entries[i];
                    memmap->entries[i] = memmap->entries[i + 1];
                    memmap->entries[i + 1] = temp;
                    shifted = true;
                }
            }
        }
    
        // Adjust index
        for(uint64_t i = 0; i < memmap->index; i++) {
            if(memmap->entries[i].end == memmap->entries[i].start) {
                memmap->index = i;
                break;
            }
        }
    
        // Re-Sort by start address
        sorted = true;
        while (sorted) {
            sorted = false;
            for (uint64_t i = 0; i < memmap->index - 1; i++) {
                if (memmap->entries[i].start > memmap->entries[i + 1].start) {
                    // Swap with next entry
                    memmap_entry_t temp = memmap->entries[i];
                    memmap->entries[i] = memmap->entries[i + 1];
                    memmap->entries[i + 1] = temp;
                    sorted = true;
                }
            }
        }
    }
    
    
}


/*
    Prints the memory map.
*/
void print_memmap(memmap_t memmap) {
    log(1, "Kernel MMAP Entries: %d\n", memmap.index);
    for (uint64_t i = 0; i < memmap.index; i++) {
        char* type;

        switch (memmap.entries[i].type) {
            case memmap_type_free:
                type = "free";
                break;
            case memmap_type_reserved:
                type = "reserved";
                break;
            case memmap_type_acpi_reclaimable:
                type = "ACPI reclaimable";
                break;
            case memmap_type_acpi_nvs:
                type = "ACPI NVS";
                break;
            case memmap_type_bad_ram:   
                type = "bad RAM";
                break;
            case memmap_type_kernel_loaded:
                type = "Kernel loaded";
                break;
            case memmap_type_kernel_data:
                type = "kernel data";
                break;
            case memmap_type_module:
                type = "module";
                break;
            case memmap_type_framebuffer:
                type = "framebuffer";
                break;
            case memmap_type_unknown:
                type = "unknown";
                break;
            default:
                type = "unknown";

        }

            
        log(0, "Kernel MMAP Entry  %x - %x (%b) - %s\n", 
            memmap.entries[i].start, 
            memmap.entries[i].end,
            memmap.entries[i].flags,
            type);
    }
}
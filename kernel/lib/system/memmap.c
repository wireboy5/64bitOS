#include "memmap.h"


extern char _kernel_end_phys;
extern char _kernel_start_phys;

memory_map_t memory_map;

memmap_entry_t create_mmap_entry(uint64_t start, uint64_t size, uint64_t flags) {
    memmap_entry_t entry;
    entry.start = start;
    entry.size = size;
    entry.flags = flags;
    return entry;
}

memory_map_t* generate_memmap(void* multiboot_info, sysinfo_t* sysinfo) {
    // Now we need to create a list of all data and structures that we are using in the kernel.
    // kernel_start and kernel_end will be useful for this.
    // We will then add all grub modules to this list.
    // And everything that grub lists in the memory map.
    // We will not be reclaiming ACPi memory yet.
    // This list will be stored in a list located at the lowest 
    // address that the memory map tells us is available.
    // The list will be able to hold 4096 entries.
    // Every entry will be ordered according to physical address.
    // The structure will consist of two 64 bit integers, the physical address and the size of the memory.
    // There will also be an 8 bit integer, which will contain flags describing the areas:
    // bit 0: if set, then this is a reserved area
    // bit 1: if set, then this is an ACPI reclaimable area
    // bit 2: if set, then this is an ACPI NVS area
    // bit 3: if set, then this is a bad ram area
    // bit 4: if set, then this is an area reserved for the kernel
    // bit 5: if set, then this is an area reserved for the modules
    // bit 6: if set, then this is an area reserved for GRUB
    // bit 7: if set, then this area is memory mapped IO or a framebuffer.
    // If none of the above are set, then this is an available area.

    // First allocate a block for the kernel
    memory_map.entries[memory_map.index] = create_mmap_entry((uint64_t)&_kernel_start_phys, (uint64_t)&_kernel_end_phys - (uint64_t)&_kernel_start_phys, MEMMAP_ENTRY_KERNEL);
    memory_map.index++;

    // Now allocate a block for the multiboot info
    memory_map.entries[memory_map.index] = create_mmap_entry((uint64_t)multiboot_info, (uint64_t)(multiboot_info), MEMMAP_ENTRY_MULTIBOOT_INFO); 
    memory_map.index++;

    // Now we return the address of the memory map for the GRUB parser to insert into.
    return &memory_map;

}

uint64_t sort_memmap(memory_map_t* mmap) {
    // Sorts the memory map by physical address. Returns the largest end address.

    

    bool sorted = true;
    while(sorted) {
        sorted = false;
        for(int i = 0; i < mmap->index - 1; i++) {
            if(mmap->entries[i].start > mmap->entries[i+1].start) {
                memmap_entry_t temp = mmap->entries[i];
                mmap->entries[i] = mmap->entries[i+1];
                mmap->entries[i+1] = temp;
                sorted = true;
            }
        }
        
    }

    uint64_t lend = 0;
    for(int i = 0; i < mmap->index; i++) {
        uint64_t end = mmap->entries[i].size + mmap->entries[i].start;
        
        if(end >= lend) {
            lend = end;
        }
    }


    return lend;
}

void condense_memmap(memory_map_t* mmap) {
    // Combines adjacent entries that are all available
    
    bool condense = true;
    while(condense) {
        condense = false;
        for(int i = 0; i < mmap->index - 1; i++) {
            if(mmap->entries[i].flags == 0 && mmap->entries[i+1].flags == 0 && mmap->entries[i].size + mmap->entries[i].start == mmap->entries[i+1].start) {
                mmap->entries[i].size += mmap->entries[i+1].size;
                mmap->entries[i+1].size = 0;
                condense = true;
            }
        }
    }

    // Now that we have condensed the memory map, we need to sort out all empty entries.

    bool sort = true;
    while(sort) {
        sort = false;
        for(int i = 0; i < mmap->index - 1; i++) {
            if(mmap->entries[i].size == 0) {
                // Swap this and next
                memmap_entry_t temp = mmap->entries[i];
                mmap->entries[i] = mmap->entries[i+1];
                mmap->entries[i+1] = temp;
            }
        }
    }

    // Now all of the empty entries should be at the end of the list.
    // Lets calculate and set the new index
    uint64_t size = 0;
    for(uint64_t i = 0; i < mmap->index; i++) {
        if(mmap->entries[i].size != 0) {
            size ++;
        }
    }
    mmap->index = size;


    
}
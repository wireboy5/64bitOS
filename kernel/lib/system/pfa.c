#include "pfa.h"

void set_page(sysinfo_t system_info, uint64_t page);
void clear_page(sysinfo_t system_info, uint64_t page);
bool check_page(sysinfo_t system_info, uint64_t page);

/*

// Initialize the page frame allocator
void init_pfa(sysinfo_t* system_info) {
    
    // Find the first available memory region that will fit the bitmap
    uint64_t smallest_index;
    uint64_t smallest_size;
    for(uint64_t i = 0; i < system_info->memmap->index - 1; i++) {
        if(system_info->memmap->entries[i].size >= system_info->page_allocator_info.bitmap_size) {
            if(smallest_size == 0) {
                smallest_index = i;
                smallest_size = system_info->memmap->entries[i].size;
            }
            else if(system_info->memmap->entries[i].size < smallest_size) {
                smallest_index = i;
                smallest_size = system_info->memmap->entries[i].size;
            }
        }
    }

    // Now we know which entry will fit the bitmap
    // Lets add a new bitmap entry to the memmap
    system_info->memmap->entries[system_info->memmap->index] = create_mmap_entry(
        system_info->memmap->entries[smallest_index].start,
        system_info->page_allocator_info.bitmap_size,
        MEMMAP_ENTRY_PFA_BITMAP
    );
    system_info->memmap->index++;

    // Set the bitmap location
    uint64_t bitmap_location = system_info->memmap->entries[smallest_index].start;


    // And lets make the old entry smaller
    system_info->memmap->entries[smallest_index].size -= system_info->page_allocator_info.bitmap_size;
    // And move up the address
    system_info->memmap->entries[smallest_index].start += system_info->page_allocator_info.bitmap_size;

    
    // Set bitmap address
    system_info->page_allocator_info.bitmap_addr = bitmap_location;
    
    // NOTE: This following code modifies the memory map
    // Check here for any issues before checking the memory map code
    

    // Make sure all available entries end and start on a page boundary
    for(uint64_t i = 0; i < system_info->memmap->index; i++) {

        
        
        // If the entry is available
        if(system_info->memmap->entries[i].flags == MEMMAP_ENTRY_AVAILABLE) {
            // If it does not start on a page boundary
            if((system_info->memmap->entries[i].start & ~0xFFF) != system_info->memmap->entries[i].start) {
                // Get the offset
                uint64_t offset = system_info->memmap->entries[i].start - (system_info->memmap->entries[i].start & ~0xFFF);
                offset = 0x1000 - offset;

                // If rounding up the start would execeed the end
                if(system_info->memmap->entries[i].start + offset >= system_info->memmap->entries[i].start + system_info->memmap->entries[i].size) { 
                    // Mark the page as not available
                    set_page(*system_info, system_info->memmap->entries[i].start & ~0xFFF);
                } else {
                    // If not, round up
                    system_info->memmap->entries[i].start += offset;
                    system_info->memmap->entries[i].size -= offset;
                }
            }
            
            // If it does not end on a page boundary (Because the beginning is aligned, only check if size is aligned)
            if((system_info->memmap->entries[i].size & ~0xFFF) != system_info->memmap->entries[i].size) {
                // Get the offset
                uint64_t offset = system_info->memmap->entries[i].size - (system_info->memmap->entries[i].size & ~0xFFF);

                // If rounding down the end would exceed the start
                if(offset >= system_info->memmap->entries[i].size) {
                    // Mark the page as not available
                    set_page(*system_info, system_info->memmap->entries[i].start & ~0xFFF);
                } else {
                    // If not, round down
                    system_info->memmap->entries[i].size -= offset;
                }
            }
        } else {
            // If the entry is not available
            // Iterate over each page in the range
            for(uint64_t j = system_info->memmap->entries[i].start & ~0xFFF; j < system_info->memmap->entries[i].start + system_info->memmap->entries[i].size; j += 0x1000) {
                // Mark the page as not available
                set_page(*system_info, j >> 12);
            }
        }

        
    }
    
   
}



void print_bitmap(sysinfo_t system_info) {
    // Print the bitmap
    serial_print("Page Frame Allocation Bitmap:\n");
    uint64_t current_start = 0;
    uint64_t current_size = 0;
    bool current_available = check_page(system_info, 0);
    for(uint64_t i = 0; i < system_info.page_allocator_info.num_pages; i++) {
        bool av = check_page(system_info, i);

        if(av == current_available) {
            current_size++;
        } else {
            serial_print("PF BM Entry: ");
            char c[33];
            itoa(current_start, c, 16);
            serial_print(c);
            serial_print(" - ");
            itoa(current_start + (current_size << 12), c, 16);
            serial_print(c);
            serial_print(" ( Size: ");
            itoa(current_size << 12, c, 16);
            serial_print(c);
            serial_print(" )");
            serial_print(current_available ? " - Available " : " - Occupied");
            serial_print("\n");

            current_start = i << 12;
            current_size = 1;
            current_available = av;
        }
    }

    // Print last entry
    serial_print("PF BM Entry: ");
    char c[33];
    itoa(current_start, c, 16);
    serial_print(c);
    serial_print(" - ");
    itoa(current_start + (current_size << 12), c, 16);
    serial_print(c);
    serial_print(" ( Size: ");
    itoa(current_size, c, 16);
    serial_print(c);
    serial_print(" )");
    serial_print(current_available ? " - Available " : " - Occupied");
    serial_print("\n");
}






void* falloc(sysinfo_t system_info) {
    // Returns the first found available page
    for(uint64_t i = 0; i < system_info.page_allocator_info.num_pages; i++) {
        
        if(check_page(system_info, i)) {
            set_page(system_info, i);
            return (void*) (i << 12);
        }   
    }
    return NULL;
}

void ffree(sysinfo_t system_info, void* addr) {
    // Frees the page
    uint64_t page = (uint64_t) addr >> 12;
    clear_page(system_info, page);
}



void* fballoc(sysinfo_t system_info, size_t size) {
    // Allocates size pages of memory in a contigous block

    // Find the smallest fitting block
    uint64_t current_index = 0;
    uint64_t current_size = 0;
    uint64_t largest_index = 0;
    uint64_t largest_size = 0;
    bool current_available = check_page(system_info, 0);
    for(uint64_t i = 0; i < system_info.page_allocator_info.num_pages; i++) {
        bool av = check_page(system_info, i);

        if(av == current_available) {
            current_size++;
        } else {
            if(current_available) {
                if(current_size == size) {
                    largest_size = current_size;
                    largest_index = current_index;
                    break;
                } else if (current_size > size && current_size < largest_size) {
                    largest_size = current_size;
                    largest_index = current_index;
                } else if (largest_size == 0 && current_size > size) {
                    largest_size = current_size;
                    largest_index = current_index;
                    
                }
                
            }

            current_index = i;
            current_size = 1;
            current_available = av;
        }

        
    }

    // Mark all needed pages as occupied
    for(uint64_t i = 0; i < size; i++) {
        set_page(system_info, i + largest_index);
    }


    // Return the first page
    return (void*) (largest_index << 12);
}




// Sets the bit of a page
void set_page(sysinfo_t system_info, uint64_t page) {
    // Verify that the page is not out of bounds
    if(page > system_info.page_allocator_info.num_pages) {
        return;
    }

    // Get a reference to the bitmap
    uint64_t* bitmap = (uint64_t*)system_info.page_allocator_info.bitmap_addr;

    // Set the bit
    bitmap[page / 64] |= (1ULL << (page % 64));
}

// Clears the bit of a page
void clear_page(sysinfo_t system_info, uint64_t page) {
    // Verify that the page is not out of bounds
    if(page > system_info.page_allocator_info.num_pages) {
        return;
    }

    // Get a reference to the bitmap
    uint64_t* bitmap = (uint64_t*)system_info.page_allocator_info.bitmap_addr;

    // Clear the bit
    bitmap[page / 64] &= ~(1ULL << (page % 64));
}

// Checks the bit of a page
bool check_page(sysinfo_t system_info, uint64_t page) {

    // Verify that the page is not out of bounds
    if(page > system_info.page_allocator_info.num_pages) {
        return false;
    }

    // Get a reference to the bitmap
    uint64_t* bitmap = (uint64_t*)system_info.page_allocator_info.bitmap_addr;

    // Check the bit
    return !((bitmap[page / 64] >> (page % 64)) & 1ULL);
}
//*/
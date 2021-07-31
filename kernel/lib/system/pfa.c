#include "pfa.h"

void set_page(sysinfo_t* system_info, uint64_t page);
void clear_page(sysinfo_t* system_info, uint64_t page);
bool check_page(sysinfo_t* system_info, uint64_t page);



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
    uint64_t last_end = 0;
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
                    set_page(system_info, system_info->memmap->entries[i].start & ~0xFFF);
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
                    set_page(system_info, system_info->memmap->entries[i].start & ~0xFFF);
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
                set_page(system_info, j >> 12);
            }
        }

        
    }
   
}



void print_bitmap(sysinfo_t* system_info) {
    // Print the bitmap
    serial_print("Bitmap:\n");
    uint64_t current_start = 0;
    uint64_t current_size = 0;
    bool current_available = true;
    for(uint64_t i = 0; i < system_info->page_allocator_info.num_pages; i++) {
        bool av = check_page(system_info, i);

        if(av == current_available) {
            current_size++;
        } else {
            char c[33];
            serial_print(av ? "Avail: ": "Full: ");
            itoa(current_start, c, 16);
            serial_print(c);
            serial_print(" - ");
            itoa(current_start + (current_size << 12), c, 16);
            serial_print(c);
            serial_print("\n");

            current_start = i << 12;
            current_size = 1;
            current_available = av;
        }
    }
}






void* falloc(sysinfo_t* system_info) {
    // Returns the first found available page
    for(uint64_t i = 0; i < system_info->page_allocator_info.num_pages / 64; i++) {
        // If the entry is full we can just continue
        if(((uint64_t*)(system_info->page_allocator_info.bitmap_addr))[i] == 0xFFFFFFFFFFFFFFFF) {
            continue;
        }
        

        // If not, then iterate over each bit
        for(uint64_t j = 0; j < 64; j++) {
            // Check if the bit is set
            if(!((((uint64_t*)(system_info->page_allocator_info.bitmap_addr))[i] >> j) & 0x1)) {
                // Set the bit
                (((uint64_t*)(system_info->page_allocator_info.bitmap_addr))[i]) |= (0x1 << j);

                // Return the address
                return (void*)((i * 64 + j) * 0x1000);
            }
        }
        
    }
}









// Sets the bit of a page
void set_page(sysinfo_t* system_info, uint64_t page) {
    // Verify that the page is not out of bounds
    if(page > system_info->page_allocator_info.num_pages) {
        return;
    }

    // Get a reference to the bitmap
    uint64_t* bitmap = (uint64_t*)system_info->page_allocator_info.bitmap_addr;

    // Set the bit
    bitmap[page / 64] |= (1ULL << (page % 64));
}

// Clears the bit of a page
void clear_page(sysinfo_t* system_info, uint64_t page) {
    // Verify that the page is not out of bounds
    if(page > system_info->page_allocator_info.num_pages) {
        return;
    }

    // Get a reference to the bitmap
    uint64_t* bitmap = (uint64_t*)system_info->page_allocator_info.bitmap_addr;

    // Clear the bit
    bitmap[page / 64] &= ~(1ULL << (page % 64));
}

// Checks the bit of a page
bool check_page(sysinfo_t* system_info, uint64_t page) {

    // Verify that the page is not out of bounds
    if(page > system_info->page_allocator_info.num_pages) {
        return false;
    }

    // Get a reference to the bitmap
    uint64_t* bitmap = (uint64_t*)system_info->page_allocator_info.bitmap_addr;

    // Check the bit
    return (bitmap[page / 64] >> (page % 64));
}

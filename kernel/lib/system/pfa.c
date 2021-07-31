#include "pfa.h"
// Sets the bit of a page
void set_page(sysinfo_t* sysinfo, uint64_t page) {
    // Verify that the page is not out of bounds
    if(page > sysinfo->page_allocator_info.num_pages) {
        return;
    }

    // Get a reference to the bitmap
    uint64_t* bitmap = (uint64_t*)sysinfo->page_allocator_info.bitmap_addr;

    // Set the bit
    bitmap[page / 64] |= (1ULL << (page % 64));
}

// Clears the bit of a page
void clear_page(sysinfo_t* sysinfo, uint64_t page) {
    // Verify that the page is not out of bounds
    if(page > sysinfo->page_allocator_info.num_pages) {
        return;
    }

    // Get a reference to the bitmap
    uint64_t* bitmap = (uint64_t*)sysinfo->page_allocator_info.bitmap_addr;

    // Clear the bit
    bitmap[page / 64] &= ~(1ULL << (page % 64));
}

// Checks the bit of a page
bool check_page(sysinfo_t* sysinfo, uint64_t page) {

    // Verify that the page is not out of bounds
    if(page > sysinfo->page_allocator_info.num_pages) {
        return false;
    }

    // Get a reference to the bitmap
    uint64_t* bitmap = (uint64_t*)sysinfo->page_allocator_info.bitmap_addr;

    // Check the bit
    return (bitmap[page / 64] >> (page % 64));
}


// Initialize the page frame allocator
void init_pfa(sysinfo_t* sysinfo) {
    // Find the first available memory region that will fit the bitmap
    uint64_t smallest_index;
    uint64_t smallest_size;
    for(uint64_t i = 0; i < sysinfo->memmap->index - 1; i++) {
        if(sysinfo->memmap->entries[i].size >= sysinfo->page_allocator_info.bitmap_size) {
            if(smallest_size == 0) {
                smallest_index = i;
                smallest_size = sysinfo->memmap->entries[i].size;
            }
            else if(sysinfo->memmap->entries[i].size < smallest_size) {
                smallest_index = i;
                smallest_size = sysinfo->memmap->entries[i].size;
            }
        }
    }

    // Now we know which entry will fit the bitmap
    // Lets add a new bitmap entry to the memmap
    sysinfo->memmap->entries[sysinfo->memmap->index] = create_mmap_entry(
        sysinfo->memmap->entries[smallest_index].start,
        sysinfo->page_allocator_info.bitmap_size,
        MEMMAP_ENTRY_PFA_BITMAP
    );
    sysinfo->memmap->index++;

    // Set the bitmap location
    uint64_t bitmap_location = sysinfo->memmap->entries[smallest_index].start;


    // And lets make the old entry smaller
    sysinfo->memmap->entries[smallest_index].size -= sysinfo->page_allocator_info.bitmap_size;
    // And move up the address
    sysinfo->memmap->entries[smallest_index].start += sysinfo->page_allocator_info.bitmap_size;

    
    // Set bitmap address
    sysinfo->page_allocator_info.bitmap_addr = bitmap_location;
    

    // Iterate over the memory map and for all non-available memory areas, set as used.
    // If it is available, set as free.
    // If an available area and a non-available area are adjacent and occupy the same page
    // then truncate the available area.
    uint64_t last_end = 0;
    for(uint64_t i = 0; i < sysinfo->memmap->index; i++) {

        // If we are not adjacent
        if(sysinfo->memmap->entries[i].start > last_end) {
            char c[33];
            itoa(last_end, c, 16);
            serial_print(c);
            serial_print(" - ");
            itoa(sysinfo->memmap->entries[i].start, c, 16);
            serial_print(c);
            serial_print("\n");
        }

        last_end = sysinfo->memmap->entries[i].start + sysinfo->memmap->entries[i].size;
        
        // If the entry is available
        if(sysinfo->memmap->entries[i].flags == MEMMAP_ENTRY_AVAILABLE) {
            // If it does not start on a page boundary
            if((sysinfo->memmap->entries[i].start & ~0xFFF) != sysinfo->memmap->entries[i].start) {
                // Get the offset
                uint64_t offset = sysinfo->memmap->entries[i].start - (sysinfo->memmap->entries[i].start & ~0xFFF);
                offset = 0x1000 - offset;

                // If rounding up the start would execeed the end
                if(sysinfo->memmap->entries[i].start + offset >= sysinfo->memmap->entries[i].start + sysinfo->memmap->entries[i].size) { 
                    // Mark the page as not available
                    set_page(sysinfo, sysinfo->memmap->entries[i].start & ~0xFFF);
                } else {
                    // If not, round up
                    sysinfo->memmap->entries[i].start += offset;
                    sysinfo->memmap->entries[i].size -= offset;
                }
            }
            
            // If it does not end on a page boundary (Because the beginning is aligned, only check if size is aligned)
            if((sysinfo->memmap->entries[i].size & ~0xFFF) != sysinfo->memmap->entries[i].size) {
                // Get the offset
                uint64_t offset = sysinfo->memmap->entries[i].size - (sysinfo->memmap->entries[i].size & ~0xFFF);

                // If rounding down the end would exceed the start
                if(offset >= sysinfo->memmap->entries[i].size) {
                    // Mark the page as not available
                    set_page(sysinfo, sysinfo->memmap->entries[i].start & ~0xFFF);
                } else {
                    // If not, round down
                    sysinfo->memmap->entries[i].size -= offset;
                }
            }
        } else {
            // If the entry is not available
            // Iterate over each page in the range
            for(uint64_t j = sysinfo->memmap->entries[i].start & ~0xFFF; j < sysinfo->memmap->entries[i].start + sysinfo->memmap->entries[i].size; j += 0x1000) {
                // Mark the page as not available
                set_page(sysinfo, j >> 12);
            }
        }

        
    }

    // Print the bitmap
    serial_print("Bitmap:\n");
    uint64_t current_start = 0;
    uint64_t current_size = 0;
    bool current_available = true;
    for(uint64_t i = 0; i < sysinfo->page_allocator_info.num_pages; i++) {
        bool av = check_page(sysinfo, i);

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



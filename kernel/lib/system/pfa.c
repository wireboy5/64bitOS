#include "pfa.h"


// Initialize the page frame allocator
void init_pfa(sysinfo_t sysinfo) {
    // Find the first available memory region that will fit the bitmap
    uint64_t smallest_index;
    uint64_t smallest_size;
    for(uint64_t i = 0; i < sysinfo.memmap->index - 1; i++) {
        if(sysinfo.memmap->entries[i].size >= sysinfo.page_allocator_info.bitmap_size) {
            if(smallest_size = 0) {
                smallest_index = i;
                smallest_size = sysinfo.memmap->entries[i].size;
            }
            else if(sysinfo.memmap->entries[i].size < smallest_size) {
                smallest_index = i;
                smallest_size = sysinfo.memmap->entries[i].size;
            }
        }
    }

    // Now we know which entry will fit the bitmap
    // Lets add a new bitmap entry to the memmap
    sysinfo.memmap->entries[sysinfo.memmap->index] = create_mmap_entry(
        sysinfo.memmap->entries[smallest_index].size,
        sysinfo.page_allocator_info.bitmap_size,
        MEMMAP_ENTRY_PFA_BITMAP
    );
    sysinfo.memmap->index++;

    // And lets make the old entry smaller
    //sysinfo.memmap->entries[smallest_index].size -= sysinfo.page_allocator_info.bitmap_size;
    // And move up the address
    //sysinfo.memmap->entries[smallest_index].start += sysinfo.page_allocator_info.bitmap_size;

}
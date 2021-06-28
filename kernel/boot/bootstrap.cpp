#include <kernel/lib/serial.h>
#include <kernel/lib/string.h>
#include <kernel/boot/getinfo.h>
#include <stdint.h>
#define koffset 0xFFFFFFFF80000000

extern "C" void load_cr3(uint64_t addr);

struct page_table {
    uint64_t entries[512];
};

struct page_directory {
    uint64_t entries[512];
};

struct page_directory_pointer_table {
    uint64_t entries[512];
};

struct page_map_level_4 {
    uint64_t entries[512];
};

// We have four page directories
// And as such, we have 2048 page tables
// Which each have 512 entries
// Giving us 1048576 page table entries
page_table page_tables[2048];
page_directory page_directories[4];
page_directory_pointer_table pdpts[3];
page_map_level_4 pml4;

/*
    Function which maps all provided page tables
*/
void map_page_tables(page_table* page_tables, uint64_t count, uint64_t offset, uint16_t flags) {
    

    
    // Start mapping each table
    for(uint64_t i = 0; i < count; i++) {
        for(uint64_t j = 0; j < 512; j++) {
            uint64_t pt =  (((i * 0x200000) + j * 0x1000) & ~0xFFF) | (flags & 0xFFF);
            
            page_tables[i].entries[j] = (
               pt
            );
        }
    }
}



void map_page_directories(page_directory* page_directories, page_table* page_tables, uint64_t count, uint16_t flags) {
    // Make sure we only have flags
    flags &= 0xFFF;
                
    // For every page directory, move in the address of the respective entries
    for(uint64_t i = 0; i < count; i++) {
        // Map a single directory
        for(uint64_t j = 0; j < 512; j++) {

            // Calculate page table offset
            // There are 512 each directory
            uint64_t pto = i * sizeof(page_directory);
            // and then we add the index in the directory
            pto += j * sizeof(page_table);

            // Grab the page table address
            uint64_t pta = (uint64_t)page_tables + pto;

            // Remove the offset
            pta -= koffset;

            
            page_directories[i].entries[j] = (
                pta | flags
            );
        }
    }
}

page_directory_pointer_table new_pdpt(page_directory* pds, uint64_t count, uint64_t offset, uint16_t flags) {
    page_directory_pointer_table pdpt;
    for (uint64_t i = 0; i < count; i++) {
        pdpt.entries[offset + i] = ((((uint64_t)&pds[i] - koffset) & ~0xFFF) | (flags & 0xFFF));
    }
    return pdpt;
}

extern "C" void bootstrap_paging(void *mboot) {
    // Create extra serial newline
    serial_print("\n");


    serial_print("[ BOOTSTRAP ] Mapping Page Tables\n");
    map_page_tables(&page_tables[0], 512, 0, 0b11);
      
    serial_print("[ BOOTSTRAP ] Mapping Page Directories\n");
    map_page_directories(&page_directories[0], &page_tables[0], 1, 0b11);
    
    // Map framebuffer page directory
    map_page_directories(&page_directories[1], &page_tables[512], 1, 0b11);

    serial_print("[ BOOTSTRAP ] Mapping PDPTs\n");

    // Lower half
    pdpts[0] = new_pdpt(&page_directories[0], 1, 0, 0b11);
    
    // Higher half
    pdpts[1] = new_pdpt(&page_directories[0], 1, 510, 0b11);
    
    

    serial_print("[ BOOTSTRAP ] Mapping PML4\n");
    

    // Map pml4
    pml4.entries[0] = ((uint64_t)&pdpts[0] - koffset) | (0b11);
    pml4.entries[511] = (((uint64_t)&pdpts[1] - koffset) | (0b11));
    
    // Load pml4
    load_cr3((uint64_t)(&pml4) - koffset | 0b11);
    for(;;);
}
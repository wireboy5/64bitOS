#include "bootstrap.h"

extern "C" uint64_t _kernel_start;
extern "C" uint64_t kernOffset;
extern "C" uint64_t code;
extern "C" uint64_t rodata;
extern "C" uint64_t data;
extern "C" uint64_t bss;
extern "C" uint64_t _kernel_end;

extern "C" void setupPaging(uint32_t);

extern "C" int bootstrap_kernel(multiboot_info* mboot_info){
    char* vidmem = (char*)0xB8000;
    
    // So, every address that has been included by the linker
    // Is at physical address address - kernOffset
    // As these are virtual addresses.

    // Where we are going to put the PML4
    uint64_t pml4Loci = _kernel_end;
    pml4Loci &= 0xFFFFF000; // Align it to a page
    pml4Loci += 0x2000;     // Add 2 pages to it
    uint32_t* pml4Loc = (uint32_t*)pml4Loci;

    // Where we are going to put the PDPT
    uint32_t* pdptLoc = (uint32_t*)(&pml4Loc + 0x1000);

    // Where we are going to put the PDT
    uint32_t* pdtLoc = (uint32_t*)(&pdptLoc + 0x1000);

    // And where the page table will be located
    uint32_t* ptLoc = (uint32_t*)(&pdtLoc + 0x1000);


    // We are currently in protected mode, and are thus limited
    // We are simply going to identity map the lowest 2MB of memory

    // We only want to map the first entry
    // of PML4, PDPT, and PDT
    *pml4Loc  = 0x2003;
    *pdptLoc  = 0x2003;
    *pdtLoc   = 0x2003;
    
    int i = 0;
    uint64_t addr = 0x00000003;
    while(i < 512){
        ptLoc[i] = addr;
        addr += 0x1000;
        i++;
    }

    setupPaging(pml4Loci);

    return 0;
}
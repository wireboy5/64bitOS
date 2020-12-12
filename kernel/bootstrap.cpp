#include "bootstrap.h"

extern "C" uint64_t _kernel_start;
extern "C" uint64_t kernOffset;
extern "C" uint64_t code;
extern "C" uint64_t rodata;
extern "C" uint64_t data;
extern "C" uint64_t bss;
extern "C" uint64_t _kernel_end;

extern "C" int bootstrap_kernel(multiboot_info* mboot_info){
    char* vidmem = (char*)0xB8000;
    
    // So, every address that has been included by the linker
    // Is at physical address address - kernOffset
    // As these are virtual addresses.

    // Virtual addresses _kernel_start to _kernel_end need to be mapped
    // To (_kernel_start - kernOffset) and (_kernel_end - kernOffset)
    // And 0x0 to (_kernel_start-1) needs to be mapped to
    // (_kernel_end - kernOffset) to 0xFFFFFFFF

    
    
    // Lets get the memory map from GRUB
    mmap_entry_t* entry = (mmap_entry_t*)mboot_info->mmap_addr;
	mmap_entry_t* largest;
    uint64_t memSize;
    while(entry < (mmap_entry_t*)(mboot_info->mmap_addr + mboot_info->mmap_length)) {
        
		if(entry->type == 1){
            if(
                (uint64_t)entry->length_high<<32|entry->length_low
                >
                (uint64_t)largest->length_high<<32|largest->length_low
            ){
                largest = entry;
            }
        }
        memSize += (uint64_t)largest->length_high<<32|largest->length_low;
        entry = (mmap_entry_t*) ((uint64_t) entry + entry->size + sizeof(entry->size));
	}

    // We have the memory end address (memSize)
    // We also have the kernel end address (_kernel_end)
    // So now we need to map from address 0x0 to  (0xFFFFFFFF - kernelOffset)
    // We also need to map from address kernelOffset to 0xFFFFFFFF
    // Lets define the start and end addresses of each

    uint64_t kernelStartVirtual = kernOffset;
    uint64_t kernelEndVirtual   = 0xFFFFFFFF;

    uint64_t freeStartVirtual   = 0;
    uint64_t freeEndVirtual     = kernOffset - 1;


    // Lets map from 0 to the size of kernel offset to end
    uint64_t kernelStartPhysical= 0x0;
    uint64_t kernelEndPhysical  = kernelEndVirtual - kernelStartVirtual;

    uint64_t freeStartPhysical  = kernelEndPhysical+1;
    uint64_t freeEndPhysical    = memSize;

    // Test bitmap real quick
    bitmap test(8);

    test.set(8);

    bool isOk = test.check(8);
    
    if(isOk){
        vidmem[0] = 'K';
    }

    return 0;
}
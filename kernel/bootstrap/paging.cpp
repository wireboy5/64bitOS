#include "paging.h"



extern "C" uint64_t _kernel_end;
uint64_t placementAddress = _kernel_end;

uint64_t placementMalloc(uint64_t size, uint8_t align, uint64_t *phys_addr){
    if (align == 1 && (placementAddress & 0xFFFFF000)) {
        placementAddress &= 0xFFFFF000;
        placementAddress += 0x1000;
    }
    
    if (phys_addr) *phys_addr = placementAddress;

    uint32_t ret = placementAddress;
    placementAddress += size;
    return ret;
}



#include "bootstrap.h"

extern "C" uint64_t _kernel_start;
extern "C" uint64_t kernOffset;
extern "C" uint64_t code;
extern "C" uint64_t rodata;
extern "C" uint64_t data;
extern "C" uint64_t bss;
extern "C" uint64_t _kernel_end;

extern "C" int bootstrap_kernel(){
    char* vidmem = (char*)0xB8000;
    
    // So, every address that has been included by the linker
    // Is at physical address address - kernOffset
    // As these are virtual addresses
    // 

    return 0;
}
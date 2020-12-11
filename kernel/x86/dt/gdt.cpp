#include "gdt.h"
gdt_entry_t gdtEntries[GDTSIZE];
gdt_ptr_t gdtPtr;


void initializeGDT(){
    // null segment
    setGDTGate(0,0,0,0,0);

    // Kernel Code Segment
    setGDTGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    // Kernel Data Segment
    setGDTGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    // Setup gdtPtr
    gdtPtr.limitHigh= (sizeof(gdt_entry_t) * GDTSIZE) - 1;
    gdtPtr.baseAddr = (uint32_t)&gdtEntries;


    // Update the gdt
    flushGDT((uint32_t)&gdtPtr);
}


void setGDTGate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran){
    // Base addres
    gdtEntries[num].baseLow     = base & 0xFFFF;
    gdtEntries[num].baseMiddle  = (base >> 16) & 0xFF;
    gdtEntries[num].baseHigh    = (base >> 24) & 0xFF;


    // Limit
    gdtEntries[num].limitLow    = (limit & 0xFFFF);

    // Granularity
    gdtEntries[num].granularity = (limit >> 16) & 0x0F;
    gdtEntries[num].granularity |= gran & 0xF0;

    // Access
    gdtEntries[num].access      = access;

}
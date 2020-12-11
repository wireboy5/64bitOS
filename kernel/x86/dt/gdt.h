#pragma once
#include <stdint.h>

#define GDTSIZE 3


struct gdt_entry_t {
    uint16_t limitLow;
    uint16_t baseLow;
    uint8_t baseMiddle;
    uint8_t access;
    uint8_t granularity;
    uint8_t baseHigh;
} __attribute__((packed));



struct gdt_ptr_t {
    uint16_t limitHigh;
    uint32_t baseAddr;
} __attribute__((packed));


extern "C" void flushGDT(uint32_t addrptr);

void initializeGDT();

void setGDTGate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);



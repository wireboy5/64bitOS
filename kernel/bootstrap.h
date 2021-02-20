#pragma once
#include <stdint.h>
struct gdt_entry_t
{
   uint16_t limit_low;           // The lower 16 bits of the limit.
   uint16_t base_low;            // The lower 16 bits of the base.
   uint8_t  base_middle;         // The next 8 bits of the base.
   uint8_t  access;              // Access flags, determine what ring this segment can be used in.
   uint8_t  granularity;
   uint8_t  base_high;           // The last 8 bits of the base.
} __attribute__((packed));

struct gdt_ptr_t
{
    uint16_t limit;
    uint32_t base;
};

gdt_entry_t gdt_entries[5];
gdt_ptr_t gdt_ptr;

static void gdt_set_gate(int32_t,uint32_t,uint32_t,uint8_t,uint8_t);


extern void gdt_flush(uint32_t);
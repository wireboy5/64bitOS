#pragma once
#include <system/sysinfo.h>
#include <stdint.h>
#include <stddef.h>


struct memmap_entry {
    uint64_t start;
    uint64_t size;
    uint8_t flags;
};
typedef struct memmap_entry memmap_entry_t;

struct memmap{
    memmap_entry_t entries[4096];
    uint32_t index = 0;
};
typedef struct memmap memory_map_t;

memory_map_t memory_map;


memmap_entry_t create_mmap_entry(uint64_t start, uint64_t size, uint64_t flags);

memory_map_t* generate_memmap(void* multiboot_info, sysinfo_t* sysinfo);
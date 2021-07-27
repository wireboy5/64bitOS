#pragma once
#include <system/sysinfo.h>
#include <stdint.h>
#include <stddef.h>







memmap_entry_t create_mmap_entry(uint64_t start, uint64_t size, uint64_t flags);

memory_map_t* generate_memmap(void* multiboot_info, sysinfo_t* sysinfo);
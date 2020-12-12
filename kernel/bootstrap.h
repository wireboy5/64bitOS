#pragma once
#include <stdint.h>
#include <stddef.h>
#include <kernel/bootstrap/paging.h>
#include <kernel/lib/multiboot.h>


typedef struct multiboot_memory_map {
	size_t size;
	uint32_t base_addr_low,base_addr_high;
	uint32_t length_low,length_high;
	uint32_t type;
} multiboot_memory_map_t;
typedef multiboot_memory_map_t mmap_entry_t;
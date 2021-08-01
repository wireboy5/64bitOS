#pragma once
#include <system/sysinfo.h>
#include <system/memmap.h>
#include <system/macros.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

// Initialize the page frame allocator
void init_pfa(sysinfo_t* sysinfo);

// Frame alloc
void* falloc(sysinfo_t system_info);

// Frame free
void ffree(sysinfo_t system_info, void* addr);

// Frame Block alloc
void* fballoc(sysinfo_t system_info, size_t size);


void print_bitmap(sysinfo_t sysinfo);
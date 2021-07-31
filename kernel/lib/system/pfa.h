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
void* falloc(sysinfo_t* system_info);

// Frame free
void ffree(void* frame);

// Frame Block alloc
void* fbvalloc(size_t size);


void print_bitmap(sysinfo_t* sysinfo);
#pragma once
#include <system/sysinfo.h>
#include <system/memmap.h>
#include <system/macros.h>

// Initialize the page frame allocator
void init_pfa(sysinfo_t sysinfo);
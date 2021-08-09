#pragma once
#include <stdbool.h>
#include <system/sysinfo.h>
#include <multiboot2.h>
#include <libk/log.h>

#include "memmap_struct.h"


/*
    Generates a memory map based off of system information.
*/
memmap_t generate_memmap(sysinfo_t sysinfo);

/*
    Shifts all zero-size entries to the end of the map, adjusts the index, and then sorts the memory map
    by start address. All "free" entries are then page aligned up the the nearest page boundary.
    Any overlapping tables are truncated according to precedence.
*/
void memmap_cleanup(memmap_t* memmap);


/*
    Prints the memory map.
*/
void print_memmap(memmap_t memmap);
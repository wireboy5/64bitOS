#pragma once
#include <stdbool.h>
#include <system/sysinfo.h>

/*
    A single entry in the memory map.
*/
struct memmap_entry {
    uint64_t    start;  // Start address of the entry
    uint64_t    end;    // End address of the entry
#define memmap_type_free 0 // Free memory
#define memmap_type_reserved 1 // Reserved memory
#define memmap_type_acpi_reclaimable 2 // ACPI reclaimable memory
#define memmap_type_acpi_nvs 3 // ACPI Non-volatile Storage
#define memmap_type_bad_ram 4 // Bad RAM region
#define memmap_type_os_loaded 5 // This entry shows where the OS is loaded
#define memmap_type_os_data 6 // This is data that the OS uses.
#define memmap_type_module 7 // This entry shows where a module is loaded
#define memmap_type_framebuffer 8 // Where a framebuffer is located
#define memmap_type_unknown 9 // Unknown memory type (Considered reserved)
    uint8_t     type;   // Type of the entry
#define memmap_flag_multiboot 0b00000001 // This entry was found in the multiboot memory map
#define memmap_flag_kernel_occupied 0b00000010 // This entry is occupied or used by the kernel
#define memmap_flag_page_rounded 0b00000100 // This entry was rounded up / down to a page boundary.
    uint8_t     flags;  // Flags of the entry
};
typedef struct memmap_entry memmap_entry_t;

/*
    The memory map.
    This only contains 4095 entries, and occupies 10 pages of memory.
*/
struct memmap {
    memmap_entry_t entries[4095];
    uint64_t memmap_index;
    uint64_t reserved;
    uint16_t reserved2;
};
typedef struct memmap memmap_t;


/*
    Generates a memory map based off of system information.
*/
memmap_t* generate_memmap(sysinfo_t sysinfo);

/*
    Shifts all zero-size entries to the end of the map, adjusts the index, and then sorts the memory map
    by start address. All "free" entries are then page aligned up the the nearest page boundary.
    Any overlapping tables are truncated according to precedence.
*/
void memmap_cleanup(memmap_t* memmap);
#pragma once
#include <screen/screen.h>
#include <io/serial.h>
#include <multiboot2.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


struct memmap_entry {
    uint64_t start;
    uint64_t size;
    uint8_t flags;
};
typedef struct memmap_entry memmap_entry_t;

struct memmap {
    memmap_entry_t entries[4096];
    uint32_t index;
};
typedef struct memmap memory_map_t;

struct multiboot_tags_header {
    uint32_t total_size;
    uint32_t reserved;
};
typedef struct multiboot_tags_header multiboot_tags_header_t;

struct sysinfo {
    framebuffer_t fb;
    multiboot_tags_header_t tags_header;
    struct multiboot_tag_mmap* grub_memmap;
    memory_map_t* memmap;
};
typedef struct sysinfo sysinfo_t;

// Retrieves the system info from the multiboot header
// and various other sources.
sysinfo_t get_sysinfo(void* multiboot_info);


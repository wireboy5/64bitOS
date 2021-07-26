#pragma once
#include <screen/screen.h>
#include <io/serial.h>
#include <multiboot2.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>

struct multiboot_tags_header {
    uint32_t total_size;
    uint32_t reserved;
};
typedef struct multiboot_tags_header multiboot_tags_header_t;

struct sysinfo {
    framebuffer_t fb;
    multiboot_tags_header_t tags_header;
    size_t mmap;
};
typedef struct sysinfo sysinfo_t;

// Retrieves the system info from the multiboot header
// and various other sources.
sysinfo_t get_sysinfo(void* multiboot_info);


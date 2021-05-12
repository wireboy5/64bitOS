#pragma once
#include <stddef.h>
#include <stdint.h>

#include <kernel/boot/multiboot2.h>
#include <kernel/lib/logging.h>
#include <kernel/lib/serial.h>
#include <kernel/lib/string.h>


struct mboot_tags_header {
    uint32_t total_size;
    uint32_t reserved;
};

struct framebuffer {
    size_t addr;
    size_t width;
    size_t height;
    size_t pitch;
    uint8_t bpp;
    uint8_t type;
};

struct sysinfo {
    mboot_tags_header tags_header;

    framebuffer fb;
};




sysinfo get_info(void* mboot_header);
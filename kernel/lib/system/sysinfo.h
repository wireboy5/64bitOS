#pragma once
#include <screen/screen.h>
#include <system/macros.h>
#include <libk/log.h>
#include <multiboot2.h>
#include <libk/string.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

struct multiboot_tags_header {
    uint32_t total_size;
    uint32_t reserved;
};
typedef struct multiboot_tags_header multiboot_tags_header_t;

struct boot_device_info {
    uint32_t biosdev; // Bios disk device
    uint32_t partition; // The top level partition
    uint32_t sub_part; // The sub partition.
};
typedef struct boot_device_info boot_device_info_t;

struct framebuffer {
    uint64_t address;
    uint64_t pitch;
    uint32_t width;
    uint32_t height;
    uint8_t  bpp;
    uint8_t  type;
    uint16_t reserved;

    union
    {
        struct
        {
            uint16_t framebuffer_palette_num_colors;
            struct multiboot_color* framebuffer_palette;
        } indexed;
        struct
        {
            uint8_t red_field_position;
            uint8_t red_mask_size;
            uint8_t green_field_position;
            uint8_t green_mask_size;
            uint8_t blue_field_position;
            uint8_t blue_mask_size;
        } rgb;
    } color;
};


typedef struct framebuffer framebuffer_t;

struct sysinfo {
    // These are initialized along side the multiboot header
    // If any of these values are NULL then multiboot did not provide them
    // and the computer may not support them. (Eg. RDSPv2 on ACPI 1.0 systems.)

    // The multiboot tag header
    multiboot_tags_header_t tags_header;

    // The number of command lines provided by bootloader.
    uint64_t num_cmdlines;

    // The name of the bootloader.
    char* bootloader_name;

    // The number of modules discovered.
    uint64_t num_modules;

    // The boot device information.
    boot_device_info_t bootdev_info;

    // The address of the memory map provided by multiboot
    uint64_t mboot_mmap_addr;

    // Pointer to the framebuffer
    framebuffer_t fb;

    // The address of the 64 bit EFI tables
    uint64_t efi64;

    // The address of the RSDPv1 table
    uint64_t RSDPv1;

    // The address of the RSDPv2 table
    uint64_t RSDPv2;

    // The address of the EFI memory map
    uint64_t efi_mmap;

    // The 64 bit EFI image handle
    uint64_t efi64_ih;

    // The load base address
    uint64_t load_base;

    
    // These will be initialized after the previous values are initialized
    // None of these should ever be NULL.

    // The kernel generated memory map
    void* mmap;

};
typedef struct sysinfo sysinfo_t;

// Retrieves the system info from the multiboot header
// and various other sources.
sysinfo_t get_sysinfo(void* multiboot_info);

/*
    Populates the system info using the multiboot header.
*/
void sysinfo_populate_multiboot_info(void* multiboot_info, sysinfo_t* info);

/*
    Populates sysinfo further using already discovered information
*/
void sysinfo_populate_derived(sysinfo_t* sysinfo);
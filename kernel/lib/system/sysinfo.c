#include "sysinfo.h"
#include <system/memmap.h>

void parse_framebuffer(sysinfo_t* info, struct multiboot_tag_framebuffer* fb) {
    info->fb.address = fb->common.framebuffer_addr;
    info->fb.pitch = fb->common.framebuffer_pitch;
    info->fb.width = fb->common.framebuffer_width;
    info->fb.height = fb->common.framebuffer_height;
    info->fb.bpp = fb->common.framebuffer_bpp;
    info->fb.type = fb->common.framebuffer_type;
    if(info->fb.type == 0) {
        info->fb.color.indexed.framebuffer_palette_num_colors = fb->framebuffer_palette_num_colors;
        info->fb.color.indexed.framebuffer_palette = &(fb->framebuffer_palette[0]);
        serial_print(" - Indexed");
    } else if (info->fb.type == 1) {
        info->fb.color.rgb.red_field_position = fb->framebuffer_red_field_position;
        info->fb.color.rgb.green_field_position = fb->framebuffer_green_field_position;
        info->fb.color.rgb.blue_field_position = fb->framebuffer_blue_field_position;
        info->fb.color.rgb.red_mask_size = fb->framebuffer_red_mask_size;
        info->fb.color.rgb.green_mask_size = fb->framebuffer_green_mask_size;
        info->fb.color.rgb.blue_mask_size = fb->framebuffer_blue_mask_size;
        serial_print(" - RGB");
    }
}

void parse_multiboot_info(void* multiboot_info, sysinfo_t* info) {

    // Grab tags header
    info->tags_header = *(multiboot_tags_header_t*)multiboot_info;

    // Tag offset address
    serial_print("Searching Multiboot Information Structure...\n");

    

    // Initial tag
    struct multiboot_tag* tag;
    
    // Iterate through tags
    for (tag = (struct multiboot_tag*)((size_t)multiboot_info + 8);
        tag->type != MULTIBOOT_TAG_TYPE_END;
        tag = (struct multiboot_tag*)((uint8_t*)tag + ((tag->size + 7) & ~7))) {

        // Print tag info
        char tsize[33];
        char ttype[33];
        itoa(tag->size, tsize, 10);
        itoa(tag->type, ttype, 10);
        serial_print("Found tag of size: ");
        serial_print(tsize);
        serial_print(" type: ");
        serial_print(ttype);
        
        // Switch on tag type
        switch (tag->type) {
            case MULTIBOOT_TAG_TYPE_FRAMEBUFFER: { // Framebuffer
                // Create a framebuffer pointer to the tag
                struct multiboot_tag_framebuffer_common* fb_common = (struct multiboot_tag_framebuffer_common*)tag;

                // Load proper framebuffer tag
                if(fb_common->framebuffer_type != 2) {
                    // If RGB or indexed, then use it as a framebuffer
                    serial_print(" - framebuffer");
                    struct multiboot_tag_framebuffer* fb = (struct multiboot_tag_framebuffer*)tag;
                    parse_framebuffer(info, fb);
                } else {
                    // TODO: Implement EGA
                    serial_print(" - EGA Text");
                }

                // Load the framebuffer into the memory map
                uint64_t fb_size = fb_common->framebuffer_pitch * fb_common->framebuffer_height;
                
                info->memmap->entries[info->memmap->index] = create_mmap_entry(fb_common->framebuffer_addr,
                    fb_size, 0b10000000);
                info->memmap->index++;
                break;
            }
            case MULTIBOOT_TAG_TYPE_MMAP: { // Memory Map
                // Cast to memory map tag
                struct multiboot_tag_mmap* mmap = (struct multiboot_tag_mmap*)tag;

                // Print that this is a MMAP
                serial_print(" - mmap\n");


                // Load mmap address into system info
                info->grub_memmap = mmap;

                for (struct multiboot_mmap_entry* entry = (struct multiboot_mmap_entry*) ((uintptr_t)info->grub_memmap + sizeof(struct multiboot_tag_mmap));
                    (uintptr_t)entry - (uintptr_t)info->grub_memmap < info->grub_memmap->size; entry = (struct multiboot_mmap_entry*)((uintptr_t)entry + info->grub_memmap->entry_size)) {
                    char c[33];


                    serial_print("MMAP Entry: ");

                    itoa(entry->addr >> 32, c, 16);
                    serial_print(c);
                    itoa(entry->addr, c, 16);
                    serial_print(c);
                    serial_print(" - ");
                    uint64_t end_addr = entry->addr + entry->len;
                    itoa(end_addr >> 32, c, 16);
                    serial_print(c);
                    itoa(end_addr, c, 16);
                    serial_print(c);

                    uint8_t flags;

                    // Switch on type
                    switch (entry->type) {
                        case MULTIBOOT_MEMORY_AVAILABLE:
                            serial_print(" - Available\n");
                            flags = 0b00000000;
                            break;
                        case MULTIBOOT_MEMORY_RESERVED:
                            serial_print(" - Reserved\n");
                            flags = 0b00000001;
                            break;
                        case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
                            serial_print(" - ACPI reclaimable\n");
                            flags = 0b00000010;
                            break;
                        case MULTIBOOT_MEMORY_NVS:
                            serial_print(" - ACPI NVS\n");
                            flags = 0b00000100;
                            break;
                        case MULTIBOOT_MEMORY_BADRAM:
                            serial_print(" - Bad ram\n");
                            flags = 0b00001000;
                            break;
                        default:
                            serial_print(" - Unknown\n");
                            flags = 0b00000000; // Default to available
                    }

                    // Add to memmap
                    //info->memmap->entries[info->memmap->index] = create_mmap_entry(entry->addr, entry->len, flags);
                    //info->memmap->index++;
                    

                    
                }

                break;
            }
            case MULTIBOOT_TAG_TYPE_MODULE: { // Module
                // Cast to module tag
                struct multiboot_tag_module* module = (struct multiboot_tag_module*)tag;

                // Print that this is a module
                serial_print(" - module");
                serial_print(" - ");
                serial_print(module->cmdline);

                // Add module to memory map
                info->memmap->entries[info->memmap->index] = create_mmap_entry(module->mod_start, module->mod_end - module->mod_start, 0b00100000);
                info->memmap->index++;
                break;
            }
            default:
                break;
        }

        serial_print("\n");
    }
}

sysinfo_t get_sysinfo(void* multiboot_info) {
    sysinfo_t sysinfo;

    // Generate a new memmap
    sysinfo.memmap = (void*)generate_memmap(multiboot_info, &sysinfo);

    // Parse multiboot info
    parse_multiboot_info(multiboot_info, &sysinfo);

    // Print memory map
    for(uint32_t i = 0; i < sysinfo.memmap->index; i++) {
        memmap_entry_t entry = sysinfo.memmap->entries[i];
        serial_print("OS MM Entry: ");
        char c[33];

        itoa(entry.start >> 32, c, 16);
        serial_print(c);
        itoa(entry.start & 0xFFFFFFFF, c, 16);
        serial_print(c);
        serial_print(" - ");
        uint64_t end_addr = (uint64_t)entry.start + (uint64_t)entry.size;
        
        itoa(end_addr >> 32, c, 16);
        serial_print(c);
        itoa(end_addr & 0xFFFFFFFF, c, 16);
        serial_print(c);

        serial_print("(");
        itoa(entry.size >> 32, c, 16);
        serial_print(c);
        itoa(entry.size, c, 16);
        serial_print(c);
        serial_print(")");

        
        

        if (entry.flags == 0b10000000) {
            serial_print(" - Framebuffer");
        } else if (entry.flags == 0b01000000) {
            serial_print(" - GRUB");
        } else if (entry.flags == 0b00100000) {
            serial_print(" - Module");
        } else if (entry.flags == 0b00010000) {
            serial_print(" - Kernel");
        } else if (entry.flags == 0b00001000) {
            serial_print(" - Bad Ram");
        } else if (entry.flags == 0b00000100) {
            serial_print(" - ACPI NVS");
        } else if (entry.flags == 0b00000010) {
            serial_print(" - ACPI Reclaimable");
        } else if (entry.flags == 0b00000001) {
            serial_print(" - Reserved");
        } else {
            serial_print(" - Unknown/Available");
        }
        serial_print("\n");
    }

    return sysinfo;
}
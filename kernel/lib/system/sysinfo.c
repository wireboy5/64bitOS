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
    serial_print("Searching Multiboot Information Structure...");

    

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
                memory_map.entries[memory_map.index] = create_mmap_entry(fb_common->framebuffer_addr,
                    fb_common->framebuffer_pitch * fb_common->framebuffer_height, 0b00010000);
                memory_map.index++;
                break;
            }
            case MULTIBOOT_TAG_TYPE_MMAP: { // Memory Map
                // Cast to memory map tag
                struct multiboot_tag_mmap* mmap = (struct multiboot_tag_mmap*)tag;

                // Print that this is a MMAP
                serial_print(" - mmap");


                // Load mmap address into system info
                info->grub_memmap = mmap;
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
    sysinfo.memmap_addr = (void*)generate_memmap(multiboot_info, &sysinfo);

    // Parse multiboot info
    parse_multiboot_info(multiboot_info, &sysinfo);

    

    return sysinfo;
}
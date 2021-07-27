#include "sysinfo.h"

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
                break;
            }
            case MULTIBOOT_TAG_TYPE_MMAP: { // Memory Map
                // Cast to memory map tag
                struct multiboot_tag_mmap* mmap = (struct multiboot_tag_mmap*)tag;

                // Print that this is a MMAP
                serial_print(" - mmap");


                // Load mmap address into system info
                info->mmap = mmap;
            }
            default:
                break;
        }

        serial_print("\n");
    }
}

sysinfo_t get_sysinfo(void* multiboot_info) {
    sysinfo_t sysinfo;

    // Parse multiboot info
    parse_multiboot_info(multiboot_info, &sysinfo);

    // Now we need to create a list of all data and structures that we are using in the kernel.
    // kernel_start and kernel_end will be useful for this.
    // We will then add all grub modules to this list.
    // And everything that grub lists in the memory map.
    // We will not be reclaiming ACPi memory yet.
    // This list will be stored in a list located at the lowest 
    // address that the memory map tells us is available.
    // The list will be able to hold 4096 entries, or the size of the lowest memory area. Whichever is smallest.
    // Every entry will be ordered according to physical address.
    // The structure will consist of two 64 bit integers, the physical address and the size of the memory.
    // There will also be an 8 bit integer, which will contain flags describing the areas:
    // bit 0: if set, then this is a reserved area
    // bit 1: if set, then this is an ACPI reclaimable area
    // bit 2: if set, then this is an ACPI NVS area
    // bit 3: if set, then this is a bad ram area
    // bit 4: if set, then this is an area reserved for the kernel
    // bit 5: if set, then this is an area reserved for the modules
    // If none of the above are set, then this is an available area.

    return sysinfo;
}
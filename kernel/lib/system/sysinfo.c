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

    // Log that we are searching the information structure
    log(0, "Searching Multiboot Information Structure...\n");

    

    // Initial tag
    struct multiboot_tag* tag;
    
    // Iterate through tags
    for (tag = (struct multiboot_tag*)((size_t)multiboot_info + 8);
        tag->type != MULTIBOOT_TAG_TYPE_END;
        tag = (struct multiboot_tag*)((uint8_t*)tag + ((tag->size + 7) & ~7))) {
        
        // Tag type
        char* tag_type = "Unknown";
        
        // Switch on tag type
        switch (tag->type) {
            case MULTIBOOT_TAG_TYPE_CMDLINE:
                tag_type = "Command Line";
                break;
            case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
                tag_type = "Boot Loader Name";
                break;
            case MULTIBOOT_TAG_TYPE_MODULE:
                tag_type = "Module";
                break;
            case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
                tag_type = "Basic Memory Information";
                break;
            case MULTIBOOT_TAG_TYPE_BOOTDEV:
                tag_type = "Boot Device";
                break;
            case MULTIBOOT_TAG_TYPE_MMAP:
                tag_type = "Memory Map";
                break;
            case MULTIBOOT_TAG_TYPE_VBE:
                tag_type = "VBE";
                break;
            case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
                tag_type = "Framebuffer";
                break;
            case MULTIBOOT_TAG_TYPE_ELF_SECTIONS:
                tag_type = "ELF Sections";
                break;
            case MULTIBOOT_TAG_TYPE_APM:
                tag_type = "APM";
                break;
            case MULTIBOOT_TAG_TYPE_EFI32:
                tag_type = "EFI32";
                break;
            case MULTIBOOT_TAG_TYPE_EFI64:
                tag_type = "EFI64";
                break;
            case MULTIBOOT_TAG_TYPE_SMBIOS:
                tag_type = "SMBIOS";
                break;
            case MULTIBOOT_TAG_TYPE_ACPI_OLD:
                tag_type = "ACPI RDSPv1";
                break;
            case MULTIBOOT_TAG_TYPE_ACPI_NEW:
                tag_type = "ACPI RSDPv2";
                break;
            case MULTIBOOT_TAG_TYPE_NETWORK:
                tag_type = "Network";
                break;
            case MULTIBOOT_TAG_TYPE_EFI_MMAP:
                tag_type = "EFI Memory Map";
                break;
            case MULTIBOOT_TAG_TYPE_EFI_BS:
                tag_type = "EFI Boot Services";
                break;
            case MULTIBOOT_TAG_TYPE_EFI32_IH:
                tag_type = "EFI32 IH";
                break;
            case MULTIBOOT_TAG_TYPE_EFI64_IH:
                tag_type = "EFI64 IH";
                break;
            case MULTIBOOT_TAG_TYPE_LOAD_BASE_ADDR:
                tag_type = "Load Base Address";
                break;
        } 

        // Log the tag
        log(0, "Multiboot Tag Found. Size: %x Type: %d \t - %s\n", tag->size, tag->type, tag_type);
    }
}

sysinfo_t get_sysinfo(void* multiboot_info) {
    log(1, "Retrieving system information.\n");
    sysinfo_t sysinfo;

    // Parse multiboot info
    parse_multiboot_info(multiboot_info, &sysinfo);


    

    /*
    // Generate info for page frame alocator

    // Calculate number of pages
    uint64_t num_pages = (page_to & ~0xFFFULL) / 0x1000;

    // Calculate the ammount of memory that is required for the bitmap.
    uint64_t bitmap_size = (num_pages / 8); // Only counts to the last full page.

    // Round up to next multiple of 8
    bitmap_size = (bitmap_size + 7) & ~0x7;

    // Pack into structure
    page_allocator_info_t alloc_info;
    alloc_info.page_to = page_to;
    alloc_info.num_pages = num_pages;
    alloc_info.bitmap_size = bitmap_size;

    // Save to sysinfo
    sysinfo.page_allocator_info = alloc_info;
    //*/
    
   

    
    



    return sysinfo;
}
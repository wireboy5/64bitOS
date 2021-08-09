#include "sysinfo.h"
#include <libk/memory/memmap.h>


void sysinfo_populate_multiboot_info(void* multiboot_info, sysinfo_t* info) {

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

                // Increment num_cmdlines
                info->num_cmdlines++;

                break;
            case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
                tag_type = "Boot Loader Name";
                
                // Cast tag
                struct multiboot_tag_string* tag_string = (struct multiboot_tag_string*)tag;

                // Set the boot loader name
                info->bootloader_name = tag_string->string;

                break;
            case MULTIBOOT_TAG_TYPE_MODULE:
                tag_type = "Module";

                // Increment num_modules
                info->num_modules++;

                break;
            case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
                tag_type = "Basic Memory Information";
                // Ignore this tag
                break;
            case MULTIBOOT_TAG_TYPE_BOOTDEV:
                tag_type = "Boot Device";
                
                // Cast the tag
                struct multiboot_tag_bootdev* bootdev = (struct multiboot_tag_bootdev*)tag;

                // Set boot device info
                info->bootdev_info.biosdev = bootdev->biosdev;
                info->bootdev_info.partition = bootdev->slice;
                info->bootdev_info.sub_part = bootdev->part;

                break;
            case MULTIBOOT_TAG_TYPE_MMAP:
                tag_type = "Memory Map";

                // Set the address of the memory map
                info->mboot_mmap = (struct multiboot_tag_mmap*)tag;
                
                break;
            case MULTIBOOT_TAG_TYPE_VBE:
                tag_type = "VBE";
                // Ignore for now
                break;
            case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
                // Cast tag
                struct multiboot_tag_framebuffer* fb = (struct multiboot_tag_framebuffer*)tag;

                // Set framebuffer info
                info->fb.address = fb->common.framebuffer_addr;
                info->fb.pitch = fb->common.framebuffer_pitch;
                info->fb.width = fb->common.framebuffer_width;
                info->fb.height = fb->common.framebuffer_height;
                info->fb.bpp = fb->common.framebuffer_bpp;
                info->fb.type = fb->common.framebuffer_type;

                if(info->fb.type == 0) {
                    info->fb.color.indexed.framebuffer_palette_num_colors = fb->framebuffer_palette_num_colors;
                    info->fb.color.indexed.framebuffer_palette = &(fb->framebuffer_palette[0]);
                    tag_type = "Framebuffer (Indexed)";
                } else if (info->fb.type == 1) {
                    info->fb.color.rgb.red_field_position = fb->framebuffer_red_field_position;
                    info->fb.color.rgb.green_field_position = fb->framebuffer_green_field_position;
                    info->fb.color.rgb.blue_field_position = fb->framebuffer_blue_field_position;
                    info->fb.color.rgb.red_mask_size = fb->framebuffer_red_mask_size;
                    info->fb.color.rgb.green_mask_size = fb->framebuffer_green_mask_size;
                    tag_type = "Framebuffer (RGB)";
                } else {
                    tag_type = "Framebuffer (EGA)";
                }
                

                
                break;
            case MULTIBOOT_TAG_TYPE_ELF_SECTIONS:
                tag_type = "ELF Sections";
                // Ignore for now
                break;
            case MULTIBOOT_TAG_TYPE_APM:
                tag_type = "APM";
                // Ignore for now
                break;
            case MULTIBOOT_TAG_TYPE_EFI32:
                tag_type = "EFI32";
                // Ignore this
                break;
            case MULTIBOOT_TAG_TYPE_EFI64:
                tag_type = "EFI64";

                // Cast tag
                struct multiboot_tag_efi64* efi64 = (struct multiboot_tag_efi64*)tag;

                // Set address of efi tables
                info->efi64 = efi64->pointer;

                break;
            case MULTIBOOT_TAG_TYPE_SMBIOS:
                tag_type = "SMBIOS";
                // Ignore for now
                break;
            case MULTIBOOT_TAG_TYPE_ACPI_OLD:
                tag_type = "ACPI RSDPv1";

                // Set pointer to this tag + size of basic tag header
                info->RSDPv1 = (uint64_t)tag + 8;

                break;
            case MULTIBOOT_TAG_TYPE_ACPI_NEW:
                tag_type = "ACPI RSDPv2";

                // Same as ACPI OLD
                info->RSDPv2 = (uint64_t)tag + 8;

                break;
            case MULTIBOOT_TAG_TYPE_NETWORK:
                tag_type = "Network";
                // Ignore for now
                break;
            case MULTIBOOT_TAG_TYPE_EFI_MMAP:
                tag_type = "EFI Memory Map";

                // Set the address of the EFI memory map
                info->efi_mmap = (uint64_t)tag + 8;

                break;
            case MULTIBOOT_TAG_TYPE_EFI_BS:
                tag_type = "EFI Boot Services";
                // Ignore for now
                break;
            case MULTIBOOT_TAG_TYPE_EFI32_IH:
                tag_type = "EFI32 IH";
                // Ignore this
                break;
            case MULTIBOOT_TAG_TYPE_EFI64_IH:
                tag_type = "EFI64 IH";

                // Cast tag
                struct multiboot_tag_efi64_ih* efi64_ih = (struct multiboot_tag_efi64_ih*)tag;

                // Set address of efi Image Handle
                info->efi64_ih = efi64_ih->pointer;

                break;
            case MULTIBOOT_TAG_TYPE_LOAD_BASE_ADDR:
                tag_type = "Load Base Address";

                // Cast tag
                struct multiboot_tag_load_base_addr* load_base_addr = (struct multiboot_tag_load_base_addr*)tag;

                // Set load base address
                info->load_base = load_base_addr->load_base_addr;

                break;
        } 

        // Log the tag
        log(0, "Multiboot Tag Found. Size: %x Type: %d \t - %s\n", tag->size, tag->type, tag_type);
    }
}




void populate_derived(sysinfo_t* sysinfo) {
    /*
        Do nothing for now.
    */
}

sysinfo_t get_sysinfo(void* multiboot_info) {
    log(1, "Retrieving system information.\n");
    sysinfo_t sysinfo;


    // Parse multiboot info
    sysinfo_populate_multiboot_info(multiboot_info, &sysinfo);
    
    // Populate more system info based on multiboot info (Not memory maps)
    populate_derived(&sysinfo);
    
    // Generate memory map
    sysinfo.mmap = generate_memmap(sysinfo);
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
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
                serial_print(" - mmap");


                // Load mmap address into system info
                info->grub_memmap = mmap;

                uint64_t index = info->memmap->index;

                for (struct multiboot_mmap_entry* entry = (struct multiboot_mmap_entry*) ((uintptr_t)info->grub_memmap + sizeof(struct multiboot_tag_mmap));
                    (uintptr_t)entry - (uintptr_t)info->grub_memmap < info->grub_memmap->size; entry = (struct multiboot_mmap_entry*)((uintptr_t)entry + info->grub_memmap->entry_size)) {
                    char c[33];

                    /*
                    serial_print("MMAP Entry: ");
                    itoa(entry->addr, c, 16);
                    serial_print(c);
                    serial_print(" - ");
                    uint64_t end_addr = entry->addr + entry->len;

                    itoa(end_addr, c, 16);
                    serial_print(c);
                    //*/

                    uint8_t flags;

                    // Switch on type
                    switch (entry->type) {
                        case MULTIBOOT_MEMORY_AVAILABLE:
                            //serial_print(" - Available\n");
                            flags = 0b00000000;
                            break;
                        case MULTIBOOT_MEMORY_RESERVED:
                            //serial_print(" - Reserved\n");
                            flags = 0b00000001;
                            break;
                        case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
                            //serial_print(" - ACPI reclaimable\n");
                            flags = 0b00000010;
                            break;
                        case MULTIBOOT_MEMORY_NVS:
                            //serial_print(" - ACPI NVS\n");
                            flags = 0b00000100;
                            break;
                        case MULTIBOOT_MEMORY_BADRAM:
                            //serial_print(" - Bad ram\n");
                            flags = 0b00001000;
                            break;
                        default:
                            //serial_print(" - Unknown\n");
                            flags = 0b00000000; // Default to available
                    }

                    // Get the end of the new entry
                    uint64_t new_end = entry->addr + entry->len;
                    bool overlaps = false;
                    if(flags == 0){
                        // If available Check if it overlaps with an existing entry
                        for(uint64_t i = 0; i < index; i++) {
                            // Get the end of the current entry
                            uint64_t end = info->memmap->entries[i].start + info->memmap->entries[i].size;
                            memmap_entry_t mentry = info->memmap->entries[i];
                            if(mentry.start > entry->addr){
                                if(end < new_end) {
                                    // Overlapping
                                    overlaps = true;

                                    // Create entry for beginning of overlap
                                    memmap_entry_t begin = create_mmap_entry(entry->addr, mentry.start - entry->addr, flags);

                                    // Create entry for end of overlap
                                    memmap_entry_t end_entry = create_mmap_entry(end, new_end - end, flags);

                                    // Add each entry only if length is more than zero
                                    if(begin.size > 0) {
                                        info->memmap->entries[info->memmap->index] = begin;
                                        info->memmap->index++;
                                    }
                                    if(end_entry.size > 0) {
                                        info->memmap->entries[info->memmap->index] = end_entry;
                                        info->memmap->index++;
                                    }
                                    break;
                                }
                            }
                        }
                    }

                    if (!overlaps) {
                        // Add the entry
                        info->memmap->entries[info->memmap->index] = create_mmap_entry(entry->addr,
                            entry->len, flags);
                        info->memmap->index++;
                    }
                    

                    
                    

                    
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
            case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO: { // Basic memory info
                // Cast to basic memory info tag
                struct multiboot_tag_basic_meminfo* meminfo = (struct multiboot_tag_basic_meminfo*)tag;
                serial_print(" - basic memory info");
                
                // Save to sysinfo
                info->mem_lower = meminfo->mem_lower;
                info->mem_upper = meminfo->mem_upper;

                break;
            }
            case MULTIBOOT_TAG_TYPE_CMDLINE: { // Command line
                // Cast to command line tag (string)
                struct multiboot_tag_string* cmdline = (struct multiboot_tag_cmdline*)tag;

                // Print that this is a command line
                serial_print(" - cmdline");

                // Save to sysinfo
                info->cmdline = cmdline->string;

                break;
            }
            case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME: { // Boot loader name

                // Cast to boot loader name tag (string)
                struct multiboot_tag_string* name = (struct multiboot_tag_boot_loader_name*)tag;

                // Print that this is a boot loader name
                serial_print(" - boot loader name");

                // Save to sysinfo
                info->boot_loader_name = name->string;

                // Print the boot loader name
                serial_print(" - ");
                serial_print(info->boot_loader_name);

                break;
            }
            case MULTIBOOT_TAG_TYPE_EFI64 : { // EFI64
                // Cast to EFI64 tag
                struct multiboot_tag_efi64* efi64 = (struct multiboot_tag_efi64*)tag;

                // Print that this is an EFI64 tag
                serial_print(" - EFI64");

                // Save to sysinfo
                info->efi64 = efi64->pointer;

                break;
            }

            case MULTIBOOT_TAG_TYPE_ACPI_OLD : { // ACPI old
                // Cast to ACPI old tag
                struct multiboot_tag_old_acpi* acpi_old = (struct multiboot_tag_acpi_old*)tag;

                // Print that this is an ACPI old tag
                serial_print(" - ACPI old");

                // Save to sysinfo
                info->RSDPv1 = acpi_old;
                break;
            }
            case MULTIBOOT_TAG_TYPE_ACPI_NEW : { // ACPI new
                // Cast to ACPI new tag
                struct multiboot_tag_new_acpi* acpi_new = (struct multiboot_tag_acpi_new*)tag;

                // Print that this is an ACPI new tag
                serial_print(" - ACPI new");

                // Save to sysinfo
                info->RSDPv2 = acpi_new;
                break;
            }
            case MULTIBOOT_TAG_TYPE_EFI_MMAP : { // EFI memory map
                // Cast to EFI memory map tag
                struct multiboot_tag_efi_mmap* mmap = (struct multiboot_tag_efi_mmap*)tag;

                // Print that this is an EFI memory map tag
                serial_print(" - EFI memory map");

                // Save to sysinfo
                info->efi_memmap = mmap;

                // TODO: Augment memory map with EFI memory map

                break;
            }
            case MULTIBOOT_TAG_TYPE_LOAD_BASE_ADDR: { // Image base
                // Cast to image base tag
                struct multiboot_tag_load_base_addr* image_base = (struct multiboot_tag_image_base*)tag;
                
                // Print that this is an image base tag
                serial_print(" - image base");

                break;
            }
            case MULTIBOOT_TAG_TYPE_ELF_SECTIONS: { // ELF sections 
                // Cast to ELF sections tag
                struct multiboot_tag_elf_sections* elf_sections = (struct multiboot_tag_elf_sections*)tag;

                // Print that this is an ELF sections tag
                serial_print(" - ELF sections");

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

    // Re-sort the memory map
    sort_memmap(sysinfo.memmap);

    uint64_t sum = 0;
    // Print memory map
    for(uint32_t i = 0; i < sysinfo.memmap->index; i++) {
        memmap_entry_t entry = sysinfo.memmap->entries[i];
        serial_print("OS MM Entry: ");
        char c[33];

        itoa(entry.start, c, 16);
        serial_print(c);
        serial_print(" - ");
        uint64_t end_addr = (uint64_t)entry.start + (uint64_t)entry.size;
        
        itoa(end_addr, c, 16);
        serial_print(c);

        serial_print(" ( Size: ");
        itoa(entry.size, c, 16);
        serial_print(c);
        serial_print(" )");
        sum += entry.size;
        
        

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

    // Get last tag end address
    uint64_t last_tag_end = sysinfo.memmap->entries[sysinfo.memmap->index - 1].start + sysinfo.memmap->entries[sysinfo.memmap->index - 1].size;

    char c[33];
    itoa(last_tag_end, c, 16);
    serial_print("Total memory: ");
    serial_print(c);
    serial_print("\n");

    return sysinfo;
}
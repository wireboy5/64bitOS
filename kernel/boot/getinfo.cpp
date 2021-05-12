#include "getinfo.h"



void parse_framebuffer(sysinfo* info, multiboot_tag_framebuffer_common* tag){
    info->fb.addr   = tag->framebuffer_addr;
    info->fb.bpp    = tag->framebuffer_bpp;
    info->fb.height = tag->framebuffer_height;
    info->fb.width  = tag->framebuffer_width;
    info->fb.pitch  = tag->framebuffer_pitch;
    info->fb.type   = tag->framebuffer_type;
}

void parse_mboot_header(sysinfo* info, void* mboot_header) {
    // Grab the tags header
    info->tags_header = *(mboot_tags_header*)mboot_header;

    // Tag Offset Address
    size_t tag_offset = 8;

    log("Searching Multiboot Information Structure for useful data.");

    // Iterate over tags
    struct multiboot_tag* tag;
    size_t size;
    size = *(size_t*)mboot_header;
    for (tag = (struct multiboot_tag *) ((size_t)mboot_header + 8);
       tag->type != MULTIBOOT_TAG_TYPE_END;
       tag = (struct multiboot_tag *) ((multiboot_uint8_t *) tag 
                                       + ((tag->size + 7) & ~7)))
    {
        char tsize[33];
        char ttype[33];
        itoa(tag->size, tsize, 10);
        itoa(tag->type, ttype, 10);
        log("Found tag of size: ");
        serial_print(tsize);
        serial_print(" type: ");
        serial_print(ttype);
        

        switch(tag->type) {
            case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
                serial_print(" - framebuffer");
                parse_framebuffer(info, (multiboot_tag_framebuffer_common*)tag);
            default:
                break;
        }
        serial_print("\n");
    }
}
    

sysinfo get_info(void* mboot_header){
    sysinfo info;
    parse_mboot_header(&info, mboot_header);
    
    return info;
}
#include "getinfo.h"



void parse_framebuffer(sysinfo* info, multiboot_tag_framebuffer_common* tag){
    info->fb.addr = tag->framebuffer_addr;
    info->fb.bpp = tag->framebuffer_bpp;
    info->fb.height = tag->framebuffer_height;
    info->fb.width = tag->framebuffer_width;
    info->fb.pitch = tag->framebuffer_pitch;
}

void parse_mboot_header(sysinfo* info, void* mboot_header) {
    // Grab the tags header
    info->tags_header = *(mboot_tags_header*)mboot_header;

    // Grab the first tag
    multiboot_tag* tag = (multiboot_tag*)mboot_header+sizeof(mboot_tags_header);


    // Iterate over tags
    while(tag->type != MULTIBOOT_TAG_TYPE_END) {
        switch(tag->type) {
            case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
                parse_framebuffer(info,(multiboot_tag_framebuffer_common*)tag);
        }
        tag = (multiboot_tag*) ((((size_t)tag+tag->size) + (MULTIBOOT_TAG_ALIGN)-1) & ~((MULTIBOOT_TAG_ALIGN)-1));
    }
}

sysinfo get_info(void* mboot_header){
    sysinfo info;
    parse_mboot_header(&info, mboot_header);

    return info;
}
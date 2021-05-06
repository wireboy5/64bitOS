#include <kernel/main.h>

// Kernel offset, start, and end
extern "C" size_t kernelOffset;
extern "C" size_t _kernel_start;

// Multiboot 2 info
extern "C" size_t mboot_info;

char* fb_addr;

extern "C" int kmain () {

    // Total Size of tags
    uint32_t mboot_total_tags = *(uint32_t*)mboot_info;
    // Reserved
    uint32_t mboot_reserved = *(uint32_t*)(mboot_info+4);

    

    // The current tag
    multiboot_tag* tag;

    // The info we want

    multiboot_tag_framebuffer mboot_fb; // We want the framebuffer

    for (
        tag = (multiboot_tag*)(mboot_info+8);
        tag->type != MULTIBOOT_TAG_TYPE_END;
        tag = (multiboot_tag*)((char*)tag + ((tag->size+7) & ~7))) {
        switch(tag->type) {
            case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
                mboot_fb = *(multiboot_tag_framebuffer*)tag;
            default:
                break;
        }
    }
    
    fb_addr = (char*)mboot_fb.common.framebuffer_addr;
    fb_addr[0] = 0xFF;

    return 0;
}
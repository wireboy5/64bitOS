#pragma once
#include <stdint.h>
#include <multiboot2.h>



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
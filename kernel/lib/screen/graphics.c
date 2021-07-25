#include "graphics.h"





void set_pixel(sysinfo_t info, uint32_t x, uint32_t y, uint32_t color) {
    *((uint32_t*)info.fb.address + info.fb.pitch * y + x) = color;
}
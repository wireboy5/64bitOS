#pragma once
#include <stdint.h>
#include <kernel/lib/drivers/io.h>
#include <kernel/lib/string.h>
#include <stddef.h>
#define SCREEN_HEIGHT 25
#define SCREEN_WIDTH 80


namespace screen {
    void setch(uint8_t x, uint8_t y, char c, uint8_t attr);
    void set_cursor_position(uint8_t x, uint8_t y);
    void refresh_cursor();
    void kprint(char* text);
} // namespace screen
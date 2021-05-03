#pragma once

#include <stdint.h>
#include <kernel/lib/string.h>
#include <kernel/lib/io.h>


#define VGA_ROW 80
#define VGA_COL 25
#define VGA_TEXT_MEM 0xC00B8000

void set_cursor_location(uint16_t x, uint16_t y);
void setch(uint32_t x, uint32_t y, char c, char forecolor, char backcolor);
void clear_screen();
void put_char(char c, char foreolor, char backcolor);

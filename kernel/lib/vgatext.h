#pragma once
#include <stdint.h>
#include <kernel/lib/string.h>
#include <kernel/lib/io.h>
#include <stdarg.h>


#define SCREENHEIGHT 25
#define SCREENWIDTH 80
#define TEXTMEM 0xB8000


void setch(char c, uint32_t x, uint32_t y, uint8_t attr);

void clear_screen();

void kprint(char * s);


void set_cursor_position(uint8_t x, uint8_t y);


uint32_t get_offset(uint32_t x, uint32_t y);
uint32_t get_offset_row(uint32_t offset);
uint32_t get_offset_col(uint32_t offset);
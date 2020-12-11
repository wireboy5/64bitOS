#pragma once
#include <stdint.h>
#include <kernel/x86/libc/io.h>
#include <kernel/x86/libc/string.h>
#include <kernel/x86/mem/mem.h>
#include <stdarg.h>


#define SCREENHEIGHT 25
#define SCREENWIDTH 80
#define TEXTMEM 0xB8000


void setChar(char c, uint32_t x, uint32_t y, uint8_t attr);

void clearScreen();

void kprint(char * s);

void printf(const char* format,  ...);

void setCursorPosition(uint8_t x, uint8_t y);


uint32_t getOffset(uint32_t x, uint32_t y);
uint32_t getOffsetRow(uint32_t offset);
uint32_t getOffsetCol(uint32_t offset);
#pragma once

#include <libk/string.h>
#include <io/serial.h>

// Standard C headers that work while freestanding
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>

// Log levels
#define LL_DEBUG 0
#define LL_INFO 1
#define LL_WARN 2
#define LL_ERROR 3
#define LL_FATAL 4

// Default log level
#define LOG_LEVEL LL_DEBUG

/*
    Logs to all available sources at specified level
*/
void log(uint8_t level, char* format, ...);

/*
    Logs to all available sources at specified level.
    Accepts a va_list instead of variable arguments
*/
void vlog(uint8_t level, char* format, va_list args);

/*
    Writes a character to all sources
*/
void log_put_char(char c);

/*
    Writes a string to all sources
*/
void log_puts(char* c);
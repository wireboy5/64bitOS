#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>





void* memcpy(void* dstptr, const void* srcptr, size_t size);

void* memset(void* bufptr, int value, size_t size);
void* memmove(void* dstptr, const void* srcptr, size_t size);


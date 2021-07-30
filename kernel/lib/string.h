#pragma once
#include <stddef.h>
#include <stdint.h>

int memcmp(const void* aptr, const void* bptr, size_t size) ;
void* memcpy(void* __restrict dstptr, const void* __restrict srcptr, size_t size) ;
void* memmove(void* dstptr, const void* srcptr, size_t size);
void* memset(void* bufptr, int value, size_t size);
void swap(char *x, char *y);
char* reverse(char *buffer, int i, int j);
void strappend(char s[], char n);
void strbackspace(char s[]);
size_t strlen(char s[]);
size_t strcmp(char s1[], char s2[]);
char* itoa(uint64_t value, char* buffer, int base);
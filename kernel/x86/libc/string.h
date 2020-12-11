#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <kernel/x86/libc/math.h>

void swap(char *x, char *y);
char* reverse(char *buffer, int i, int j);

void strappend(char s[], char n);

void strbackspace(char s[]);

int strlen(char s[]);

int strcmp(char s1[], char s2[]);

char* itoa(int value, char* buffer, int base);
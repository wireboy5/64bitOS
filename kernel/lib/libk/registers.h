#pragma once

#include <stdint.h>


/*
    Reads the TSC counter.
*/
static inline void uint64_t rdtsc(void)
{
    uint32_t low, high;
    asm volatile("rdtsc":"=a"(low),"=d"(high));
    return ((uint64_t)high << 32) | low;
}

/************************************************
 * Functions to read the control registers
 ************************************************/

static inline uint32_t rdcr0(void) {
    uint32_t cr0;
    asm volatile("mov %%cr0, %0" : "=r" (cr0));
    return cr0;
}


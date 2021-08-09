#pragma once

#include <stdint.h>


/*
    Reads the TSC counter.
*/
static inline uint64_t rdtsc(void)
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

static inline uint32_t rdcr2(void) {
    uint32_t cr2;
    asm volatile("mov %%cr2, %0" : "=r" (cr2));
    return cr2;
}

static inline uint32_t rdcr3(void) {
    uint32_t cr3;
    asm volatile("mov %%cr3, %0" : "=r" (cr3));
    return cr3;
}

static inline uint32_t rdcr4(void) {
    uint32_t cr4;
    asm volatile("mov %%cr4, %0" : "=r" (cr4));
    return cr4;
}

static inline uint32_t rdcr8(void) {
    uint32_t cr8;
    asm volatile("mov %%cr8, %0" : "=r" (cr8));
    return cr8;
}

/************************************************
 * Model Specific Registers
 ************************************************/

static inline void wrmsr(uint64_t index, uint64_t value) {
    uint32_t low = value & 0xFFFFFFFF;
	uint32_t high = value >> 32;
	asm volatile (
		"wrmsr"
		:
		: "c"(msr), "a"(low), "d"(high)
	);
}

static inline uint64_t rdmsr(uint64_t index) {
    uint32_t low, high;
    asm volatile (
        "rdmsr"
        : "=a"(low), "=d"(high)
        : "c"(msr)
    );
    return ((uint64_t)high << 32) | low;
}
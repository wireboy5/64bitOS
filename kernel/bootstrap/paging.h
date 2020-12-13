#pragma once
#include <stdint.h>

uint64_t placementMalloc(uint64_t size, uint8_t align, uint64_t *phys_addr);


struct pageTableEntry {
    uint64_t present            : 1;
    uint64_t writeable          : 1;
    uint64_t userAccess         : 1;
    uint64_t writeThrough       : 1;
    uint64_t cacheDisabled      : 1;
    uint64_t accessed           : 1;
    uint64_t dirty              : 1;
    uint64_t zero               : 1;
    uint64_t global             : 1;
    uint64_t avail              : 3;
    uint64_t pageppn            : 28;
    uint64_t reserved           : 12;
    uint64_t ignored2           : 11;
    uint64_t disableExecution   : 1;
} __attribute__((packed));




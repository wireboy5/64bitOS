#pragma once
#include <stdint.h>

uint64_t placementMalloc(uint64_t size, uint8_t align, uint64_t *phys_addr);


class bitmap {
public:
    bitmap(uint64_t numbits);

    uint8_t* bits;
    uint32_t numBytes;

    // Set, Clear, Toggle, Check
    void set(uint64_t index);
    void clear(uint64_t index);
    void toggle(uint64_t index);
    bool check(uint64_t index);
};
typedef bitmap bitmap_t;
#include "paging.h"

// The bitmap class
bitmap::bitmap(uint64_t numbits){
    // Lets assume we always need one more.
    uint32_t numBytesi = ((numbits+8) / 8);

    // Now lets allocate the size
    uint64_t* physAddr;
    this->bits = (uint8_t*)placementMalloc(numBytes,1,physAddr);

    // And set the size for number of bytes
    numBytes = numBytesi;
}

void bitmap::set(uint64_t index){
    uint64_t byteNum = (index / 8);
    uint64_t bitNum  = index - (byteNum * 8);

    if(byteNum >= numBytes){
        return;
    }
    // Set the bit
    bits[byteNum] |= 1 << bitNum;
}

void bitmap::clear(uint64_t index){
    uint64_t byteNum = (index / 8);
    uint64_t bitNum  = index - (byteNum * 8);

    if(byteNum >= numBytes){
        return;
    }

    // Clear the bit
    bits[byteNum] &= ~(1 << bitNum);
}

void bitmap::toggle(uint64_t index){
    uint64_t byteNum = (index / 8);
    uint64_t bitNum  = index - (byteNum * 8);

    if(byteNum >= numBytes){
        return;
    }

    // Toggle the bit
    bits[byteNum] ^= 1 << bitNum;
}

bool bitmap::check(uint64_t index){
    uint64_t byteNum = (index / 8);
    uint64_t bitNum  = index - (byteNum * 8);

    if(byteNum >= numBytes){
        return false;
    }    

    return (bits[byteNum] >> bitNum) & 1;
}

#include <stdint.h>

#ifndef __UTILS__
#define __UTILS__

uint16_t littleEndianToBigEndian(uint16_t);

namespace Binary {
    uint8_t lower8BitsOf(uint16_t value);
    uint8_t higher8BitsOf(uint16_t value);
    bool is8bitValueNegative(uint8_t value);
    bool is16bitValueNegative(uint16_t value);
    bool is8bitValueZero(uint8_t value);
    bool is16bitValueZero(uint16_t value);
    void setLower8BitsOf16BitsValue(uint16_t *destination, uint8_t value);
    void setBitIn8BitValue(uint8_t *value, uint8_t bitNumber);
    void clearBitIn8BitValue(uint8_t *value, uint8_t bitNumber);
    void setBitIn16BitValue(uint16_t *value, uint8_t bitNumber);
    void clearBitIn16BitValue(uint16_t *value, uint8_t bitNumber);
}

#endif

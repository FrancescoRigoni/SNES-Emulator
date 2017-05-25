
#include <stdint.h>

#ifndef __UTILS__
#define __UTILS__

uint16_t littleEndianToBigEndian(uint16_t);

namespace Binary {
    uint8_t lower8BitsOf(uint16_t);
    uint8_t higher8BitsOf(uint16_t);
    uint16_t lower16BitsOf(uint32_t);
    bool is8bitValueNegative(uint8_t);
    bool is16bitValueNegative(uint16_t);
    bool is8bitValueZero(uint8_t);
    bool is16bitValueZero(uint16_t);
    void setLower8BitsOf16BitsValue(uint16_t *, uint8_t);
    void setBitIn8BitValue(uint8_t *, uint8_t);
    void clearBitIn8BitValue(uint8_t *, uint8_t);
    void setBitIn16BitValue(uint16_t *, uint8_t);
    void clearBitIn16BitValue(uint16_t *, uint8_t);

    uint8_t convert8BitToBcd(uint8_t);
    uint16_t convert16BitToBcd(uint16_t);
    bool bcdSum8Bit(uint8_t, uint8_t, uint8_t *, bool);
    bool bcdSum16Bit(uint16_t, uint16_t, uint16_t *, bool);

}

#endif


#include "utils.hpp"

uint16_t littleEndianToBigEndian(uint16_t littleEndian) {
    return littleEndian;//((littleEndian & 0x00FF) << 8) | ((littleEndian & 0xFF00) >> 8);
}

namespace Binary { 

uint8_t lower8BitsOf(uint16_t value) {
    return ((uint8_t)(value & 0xFF));
}

uint8_t higher8BitsOf(uint16_t value) {      
    return ((uint8_t)((value & 0xFF00) >> 8));
}

bool is8bitValueNegative(uint8_t value) {
    return (value & 0x80);
}

bool is16bitValueNegative(uint16_t value) { 
    return (value & 0x8000);
}

bool is8bitValueZero(uint8_t value) {
    return (value == 0x00);
}

bool is16bitValueZero(uint16_t value) { 
    return (value == 0x0000);
}

}

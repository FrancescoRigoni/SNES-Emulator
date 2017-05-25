
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

uint16_t lower16BitsOf(uint32_t value) {
    return ((uint16_t)(value & 0xFFFF));
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

void setLower8BitsOf16BitsValue(uint16_t *destination, uint8_t value) {
    *destination &= 0xFF00;
    *destination |= value;
}

void setBitIn8BitValue(uint8_t *value, uint8_t bitNumber) {
    uint8_t mask = 1 << bitNumber;
    *value = *value | mask;
}

void clearBitIn8BitValue(uint8_t *value, uint8_t bitNumber) {
    uint8_t mask = 1 << bitNumber;
    mask = mask ^ 0xFF;
    *value = *value & mask;
}

void setBitIn16BitValue(uint16_t *value, uint8_t bitNumber) {
    uint16_t mask = 1 << bitNumber;
    *value = *value | mask;
}

void clearBitIn16BitValue(uint16_t *value, uint8_t bitNumber) {
    uint16_t mask = 1 << bitNumber;
    mask = mask ^ 0xFFFF;
    *value = *value & mask;
}

uint8_t convert8BitToBcd(uint8_t val)
{
	uint8_t value = val;
	uint8_t result = 0;
	uint8_t shiftLeft = 0;
	while (value > 0) {
		uint8_t digit = value % 10;
		result |= digit << shiftLeft;
		value /= 10;
		shiftLeft += 4;
	}

  	return result;
}

uint16_t convert16BitToBcd(uint16_t val)
{
	uint16_t value = val;
	uint16_t result = 0;
	uint16_t shiftLeft = 0;
	while (value > 0) {
		uint8_t digit = value % 10;
		result |= digit << shiftLeft;
		value /= 10;
		shiftLeft += 4;
	}

  	return result;
}

bool bcdSum8Bit(uint8_t bcdFirst, uint8_t bcdSecond, uint8_t *bcdResult, bool carry) {
	uint8_t shift = 0;
	*bcdResult = 0;

	while (bcdFirst > 0 || bcdSecond > 0) {
		uint8_t digitOfFirst = bcdFirst & 0xF;
		uint8_t digitOfSecond = bcdSecond & 0xF;
		uint8_t sumOfDigits = digitOfFirst + digitOfSecond + (carry ? 1 : 0);
		carry = sumOfDigits > 9;
		if (carry) sumOfDigits += 6;
		sumOfDigits &= 0xF;
		*bcdResult |= sumOfDigits << shift;

		shift += 4;
		bcdFirst >>= shift;
		bcdSecond >>= shift;
	}

	return carry;
}

bool bcdSum16Bit(uint16_t bcdFirst, uint16_t bcdSecond, uint16_t *bcdResult, bool carry) {
	*bcdResult = 0;
	uint8_t shift = 0;
	while (bcdFirst > 0 || bcdSecond > 0) {
		uint8_t digitOfFirst = bcdFirst & 0xFF;
		uint8_t digitOfSecond = bcdSecond & 0xFF;
		uint8_t partialresult = 0;
		carry = bcdSum8Bit(digitOfFirst, digitOfSecond, &partialresult, carry);
		*bcdResult |= partialresult << shift;
		shift += 8;
		bcdFirst >>= shift;
		bcdSecond >>= shift;
	}
	return carry;
}

}

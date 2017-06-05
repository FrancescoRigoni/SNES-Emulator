
#include <cmath>

#include "Device.hpp"

Address Address::sumOffsetToAddressNoWrapAround(const Address &address, uint16_t offset) {
    Address newAddress;
    uint8_t newBank = address.getBank();
    uint16_t newOffset;
    uint32_t offsetSum = (uint32_t)(address.getOffset() + offset);
    if (offsetSum >= BANK_SIZE_BYTES) {
        ++newBank;
        newOffset = offsetSum - BANK_SIZE_BYTES;
    } else {
        newOffset = address.getOffset() + offset;
    }
    return Address(newBank, newOffset);
}

Address Address::sumOffsetToAddressWrapAround(const Address &address, uint16_t offset) {
    Address newAddress(address.getBank(), address.getOffset() + offset);
    return newAddress;
}

Address Address::sumOffsetToAddress(const Address &address, uint16_t offset) {
    // This wraps around by default
    // TODO figure out when to wrap around and when not to
    return sumOffsetToAddressWrapAround(address, offset);
}

bool Address::offsetsAreOnDifferentPages(uint16_t offsetFirst, uint16_t offsetSecond) {
    int pageOfFirst = std::floor(offsetFirst / PAGE_SIZE_BYTES);
    int pageOfSecond = std::floor(offsetSecond / PAGE_SIZE_BYTES);
    return pageOfFirst != pageOfSecond;
}

Address Address::newWithOffset(uint16_t offset) {
    return sumOffsetToAddress((const Address &)*this, offset);
}

Address Address::newWithOffsetNoWrapAround(uint16_t offset) {
    return sumOffsetToAddressNoWrapAround((const Address &)*this, offset);
}

Address Address::newWithOffsetWrapAround(uint16_t offset) {
    return sumOffsetToAddressWrapAround((const Address &)*this, offset);
}

void Address::decrementOffsetBy(uint16_t offset) {
    mOffset -= offset;
}

void Address::incrementOffsetBy(uint16_t offset) {
    mOffset += offset;
}

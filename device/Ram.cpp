#include "Ram.hpp"

Ram::Ram(MemoryMapper &memoryMapper)
{
    mRam = new uint8_t[LOWRAM_SIZE + HIGH_RAM_SIZE + EXPANDED_RAM_SIZE];
    memoryMapper.registerDevice(this);
}

Ram::~Ram()
{
    if (mRam) delete[] mRam;
}

void Ram::storeByte(const Address &address, uint8_t value) {
    uint32_t realAddress = RAW_ADDRESS(address);
    mRam[realAddress] = value;
}

uint8_t Ram::readByte(const Address &address) {
    uint32_t realAddress = RAW_ADDRESS(address);
    return mRam[realAddress];
}

bool Ram::decodeAddress(const Address &virtualAddress, Address &decodedAddress) {
    uint8_t bank = virtualAddress.getBank();
    uint16_t offset = virtualAddress.getOffset();

    uint8_t decodedBank;
    uint16_t decodedOffset;
    bool decoded = false;

    if ((bankInRange(bank, 0x00, 0x3F) || bankInRange(bank, 0x80, 0xBF) || bankIs(bank, 0x7E)) && offsetInRange(offset, 0x0000, 0x1FFF)) {
        // LowRAM
        decodedBank = 0x00;
        decodedOffset = offset;
        decoded = true;
    } else if (bankIs(bank, 0x75) && offsetInRange(offset, 0x2000, 0x7FFF)) {
        // HighRAM
        decodedBank = 0x00;
        decodedOffset = offset - 0x2000;
        decoded = true;
    } else if (bankIs(bank, 0x7F)) {
        // ExpandedRAM
        if (offsetIsInBankLow(offset)) {
            decodedBank = 0x00;
            decodedOffset = offset + 0x8000;
        } else {
            decodedBank = 0x01;
            decodedOffset = offset - 0x8000;
        }
        decoded = true;
    }

    if (decoded) {
        Address address(decodedBank, decodedOffset);
        decodedAddress = address;
    }

    return decoded;
}


#include <cstring>

#include "MockRam.hpp"

MockRam::MockRam(MemoryMapper &memoryMapper, uint32_t sizeInBytes) {
    mRam = new uint8_t[sizeInBytes]();  // This version of new initializes the memory to zero. new []().
    memoryMapper.registerDevice(this);
}

MockRam::~MockRam() {
    if (mRam) delete[] mRam;
}

void MockRam::storeByte(const Address &decodedAddress, uint8_t value) {
    mRam[RAW_ADDRESS(decodedAddress)] = value;
}

uint8_t MockRam::readByte(const Address &decodedAddress) {
    return mRam[RAW_ADDRESS(decodedAddress)];
}

bool MockRam::decodeAddress(const Address &virtualAddress, Address &decodedAddress) {
    Address address(virtualAddress.getBank(), virtualAddress.getOffset());
    decodedAddress = address;
    return true;
}

void MockRam::copyDataBlock(uint8_t *data, uint32_t offset, uint32_t count) {
    std::memcpy((void *)(mRam+offset), (const void *) data, count);
}

void MockRam::loadProgram(const TestProgram &program, const Address & address) {
    program.copyProgramBytes(mRam + RAW_ADDRESS(address));
}

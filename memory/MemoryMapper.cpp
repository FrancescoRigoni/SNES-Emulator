
#include <cmath>
#include "MemoryMapper.hpp"
#include "Log.hpp"

#define LOG_TAG "MemoryMapper"

MemoryMapper::MemoryMapper() {
    mPpuApuRegs = new uint8_t[PPU_APU_REGS_SIZE];
    mDspSuperFxRegs = new uint8_t[DSP_SUPERFX_REGS_SIZE];
    mOldStyleJoypadRegs = new uint8_t[OLD_STYLE_JOYPAD_REGS_SIZE];
    mDmaPpu2Regs = new uint8_t[DMA_PPU2_REGS_SIZE];
    // TODO: allocate only if present on cartridge
    mCartridgeSRam = new uint8_t[CARTRIDGE_SRAM_SIZE];

}

MemoryMapper::~MemoryMapper() {
    if (mPpuApuRegs) delete[] mPpuApuRegs;
    if (mDspSuperFxRegs) delete[] mDspSuperFxRegs;
    if (mOldStyleJoypadRegs) delete[] mOldStyleJoypadRegs;
    if (mDmaPpu2Regs) delete[] mDmaPpu2Regs;
    if (mCartridgeSRam) delete[] mCartridgeSRam;
}

void MemoryMapper::registerDevice(Device *device) {
    mDevices.push_back(device);
}

void MemoryMapper::storeByte(const Address &address, uint8_t value) {
    for (int i = 0; i < mDevices.size(); i++) {
        Device *device = mDevices[i];
        Address decodedAddress;
        if (device->decodeAddress(address, decodedAddress)) {
            return device->storeByte(decodedAddress, value);
        }
    }
    Log::err(LOG_TAG).str("Trying to write to unmapped address!").show();
}

void MemoryMapper::storeTwoBytes(const Address &address, uint16_t value) {
    for (int i = 0; i < mDevices.size(); i++) {
        Device *device = mDevices[i];
        Address decodedAddress;
        if (device->decodeAddress(address, decodedAddress)) {
            uint8_t leastSignificantByte = (uint8_t)(value & 0xFF);
            uint8_t mostSignificantByte = (uint8_t)((value & 0xFF00) >> 8);
            device->storeByte(decodedAddress, leastSignificantByte);
            decodedAddress.incrementBy(1);
            device->storeByte(decodedAddress, mostSignificantByte);
        }
    }
    Log::err(LOG_TAG).str("Trying to write to unmapped address!").show();
}

uint8_t MemoryMapper::readByte(const Address &address) {
    for (int i = 0; i < mDevices.size(); i++) {
        Device *device = mDevices[i];
        Address decodedAddress;
        if (device->decodeAddress(address, decodedAddress)) {
            return device->readByte(decodedAddress);
        }
    }
    Log::err(LOG_TAG).str("Trying to read from unmapped address!").show();
}

uint16_t MemoryMapper::readTwoBytes(const Address &address) {
    for (int i = 0; i < mDevices.size(); i++) {
        Device *device = mDevices[i];
        Address decodedAddress;
        if (device->decodeAddress(address, decodedAddress)) {
            uint8_t leastSignificantByte = device->readByte(decodedAddress);
            decodedAddress.incrementBy(sizeof(uint8_t));
            uint8_t mostSignificantByte = device->readByte(decodedAddress);
            uint16_t value = ((uint16_t)mostSignificantByte << 8) | leastSignificantByte;
            return value;
        }
    }
    Log::err(LOG_TAG).str("Trying to read from unmapped address!").show();
}

Address MemoryMapper::readAddressAt(const Address &address) {
    Address readAddress;
    for (int i = 0; i < mDevices.size(); i++) {
        Device *device = mDevices[i];
        Address decodedAddress;
        if (device->decodeAddress(address, decodedAddress)) {
            // Read offset
            uint8_t leastSignificantByte = device->readByte(decodedAddress);
            decodedAddress.incrementBy(sizeof(uint8_t));
            uint8_t mostSignificantByte = device->readByte(decodedAddress);
            uint16_t offset = ((uint16_t)mostSignificantByte << 8) | leastSignificantByte;
            // Read bank
            decodedAddress.incrementBy(sizeof(uint8_t));
            uint8_t bank = device->readByte(decodedAddress);
            Address address(bank, offset);
            readAddress = address;
        }
    }
    Log::err(LOG_TAG).str("Trying to read from unmapped address!").show();
    return readAddress;
}

/*
void logDecodedAddress(const char *type, uint8_t bank, uint16_t offset, uint8_t decodedBank, uint16_t decodedOffset) {
    Log &log = Log::dbg(LOG_TAG);
    log.str("Decoded address ").hex(bank, 2).str(":").hex(offset, 4);
    log.str(" to ").str(type).sp().hex(decodedBank, 2).str(":").hex(decodedOffset, 4);
    log.show();
}*/

/*
bool MemoryMapper::decodeAddress(Address, Address*);
     else if ((bankInRange(bank, 0x00, 0x3F) || bankInRange(bank, 0x80, 0xBF)) && offsetIsInBankLow(offset)) {
        if (offsetInRange(offset, 0x2100, 0x21FF)) {

            // PPU1/APU
            *decodedBank = 0x00;
            *decodedOffset = offset - 0x2100;
            logDecodedAddress("PPU1/APU Regs", bank, offset, *decodedBank, *decodedOffset);

            return mPpuApuRegs;

        } else if (offsetInRange(offset, 0x3000, 0x3FFF)) {

            // DSP, SuperFX, hardware registers
            *decodedBank = 0x00;
            *decodedOffset = offset - 0x3000;
            logDecodedAddress("DSP/SuperFX Regs", bank, offset, *decodedBank, *decodedOffset);

            return mDspSuperFxRegs;

        } else if (offsetInRange(offset, 0x4000, 0x40FF)) {

            // Old Style Joypad Registers
            *decodedBank = 0x00;
            *decodedOffset = offset - 0x4000;
            logDecodedAddress("Old Style Joypad Regs", bank, offset, *decodedBank, *decodedOffset);

            return mOldStyleJoypadRegs;

        } else if (offsetInRange(offset, 0x4200, 0x44FF)) {

            // DMA, PPU2, hardware registers
            *decodedBank = 0x00;
            *decodedOffset = offset - 0x4200;
            logDecodedAddress("DMA/PPU2 Regs", bank, offset, *decodedBank, *decodedOffset);

            return mDmaPpu2Regs;

        }
    } else if ((bankInRange(bank, 0x70, 0x7D) || bankInRange(bank, 0xF0, 0xFD)) && offsetIsInBankLow(offset)) {

        Log::err(LOG_TAG).str("Storing byte in cartridge SRAM is not supported!!").show();
        return 0;

    } else if (bankInRange(bank, 0xFE, 0xFF) && offsetIsInBankLow(offset)) {

        Log::err(LOG_TAG).str("Storing byte in cartridge SRAM is not supported!!").show();
        return 0;

    } else if (bankIs(bank, 0x75) && offsetIsInBankHigh(offset)) {

        *decodedBank = 0x00;
        *decodedOffset = offset - 0x8000;
        logDecodedAddress("ExpandedRam", bank, offset, *decodedBank, *decodedOffset);

        return mExpandedRam;

    } else {
        return 0;
    }
}*/


#include <cmath>
#include "MemoryMapper.hpp"
#include "Log.hpp"

#define LOG_TAG "MemoryMapper"

#define offsetIsInBankLow(offset) (offset < 0x8000)
#define offsetIsInBankHigh(offset) (offset >= 0x8000)

#define offsetToBankLow(offset) (offset - 0x8000)
#define offsetToBankHigh(offset) (offset + 0x8000)
#define bankIsEven(bank) (bank % 2 == 0)
#define bankInRange(bank, start, end) (bank >= start && bank <= end)
#define offsetInRange(offset, start, end) (offset >= start && offset <= end)
#define bankIs(bank, what) (bank == what)

MemoryMapper::MemoryMapper(RomReader &romReader) :
    mRomReader(romReader) {

    mLowRam = new uint8_t[LOWRAM_SIZE];
    mPpuApuRegs = new uint8_t[PPU_APU_REGS_SIZE];
    mDspSuperFxRegs = new uint8_t[DSP_SUPERFX_REGS_SIZE];
    mOldStyleJoypadRegs = new uint8_t[OLD_STYLE_JOYPAD_REGS_SIZE];
    mDmaPpu2Regs = new uint8_t[DMA_PPU2_REGS_SIZE];
    // TODO: allocate only if present on cartridge
    mCartridgeSRam = new uint8_t[CARTRIDGE_SRAM_SIZE];
    mHighRam = new uint8_t[HIGH_RAM_SIZE];
    mExpandedRam = new uint8_t[EXPANDED_RAM_SIZE];
}

MemoryMapper::~MemoryMapper() {
    if (mLowRam) delete[] mLowRam;
    if (mPpuApuRegs) delete[] mPpuApuRegs;
    if (mDspSuperFxRegs) delete[] mDspSuperFxRegs;
    if (mOldStyleJoypadRegs) delete[] mOldStyleJoypadRegs;
    if (mDmaPpu2Regs) delete[] mDmaPpu2Regs;
    if (mCartridgeSRam) delete[] mCartridgeSRam;
    if (mHighRam) delete[] mHighRam;
    if (mExpandedRam) delete[] mExpandedRam;
}

Address MemoryMapper::sumOffsetToAddressNoWrapAround(Address &address, uint16_t offset) {
    Address newAddress;
    newAddress.bank = address.bank;
    uint32_t offsetSum = (uint32_t)(address.offset + offset);
    if (offsetSum >= BANK_SIZE_BYTES) {
        ++newAddress.bank;
        newAddress.offset = offsetSum - BANK_SIZE_BYTES;
    } else {
        newAddress.offset = address.offset + offset;
    }
    return newAddress;
}

Address MemoryMapper::sumOffsetToAddressWrapAround(Address &address, uint16_t offset) {
    Address newAddress;
    newAddress.bank = address.bank;
    newAddress.offset = address.offset + offset;
    return newAddress;
}

bool MemoryMapper::offsetsAreOnDifferentPages(uint16_t offsetFirst, uint16_t offsetSecond) {
    int pageOfFirst = std::floor(offsetFirst / PAGE_SIZE_BYTES);
    int pageOfSecond = std::floor(offsetSecond / PAGE_SIZE_BYTES);
    return offsetFirst != offsetSecond;
}
        
void MemoryMapper::storeByte(Address &address, uint8_t value) {    
    uint8_t decodedBank;
    uint16_t decodedOffset;
    uint8_t *memory = decodeMemoryAddress(address.bank, address.offset, &decodedBank, &decodedOffset);
    
    uint32_t realAddress = (decodedBank * BANK_SIZE_BYTES) + decodedOffset;
    memory[realAddress] = value;
}


void MemoryMapper::storeTwoBytes(Address &address, uint16_t value) {
    uint8_t decodedBank;
    uint16_t decodedOffset;
    uint8_t *memory = decodeMemoryAddress(address.bank, address.offset, &decodedBank, &decodedOffset);
    
    uint32_t realAddress = (decodedBank * BANK_SIZE_BYTES) + decodedOffset;
    uint8_t leastSignificantByte = (uint8_t)(value & 0xFF);
    uint8_t mostSignificantByte = (uint8_t)((value & 0xFF00) >> 8);
    
    memory[realAddress] = leastSignificantByte;
    memory[realAddress+1] = mostSignificantByte;
}

uint8_t MemoryMapper::readByte(uint8_t bank, uint16_t offset) {
    uint8_t decodedBank;
    uint16_t decodedOffset;
    uint8_t *memory = decodeMemoryAddress(bank, offset, &decodedBank, &decodedOffset);
    uint32_t realAddress = (decodedBank * BANK_SIZE_BYTES) + decodedOffset;
    return memory[realAddress];
}

uint16_t MemoryMapper::readTwoBytes(uint8_t bank, uint16_t offset) {
    uint8_t decodedBank;
    uint16_t decodedOffset;
    uint8_t *memory = decodeMemoryAddress(bank, offset, &decodedBank, &decodedOffset);
    uint32_t realAddress = (decodedBank * BANK_SIZE_BYTES) + decodedOffset;
    uint8_t leastSignificantByte = memory[realAddress];
    uint8_t mostSignificantByte = memory[realAddress+1];
    uint16_t value = ((uint16_t)mostSignificantByte << 8) | leastSignificantByte;
    return value;
}

Address MemoryMapper::readAddressAt(uint8_t bank, uint16_t offset) {
    Address addressRead;
    addressRead.bank = readByte(bank, offset+2);
    addressRead.offset = readTwoBytes(bank, offset);
    return addressRead;
}

uint8_t MemoryMapper::readByte(Address address) {    
    return readByte(address.bank, address.offset);
}

uint16_t MemoryMapper::readTwoBytes(Address address) {
    uint8_t decodedBank;
    uint16_t decodedOffset;
    uint8_t *memory = decodeMemoryAddress(address.bank, address.offset, &decodedBank, &decodedOffset);
    uint32_t realAddress = (decodedBank * BANK_SIZE_BYTES) + decodedOffset;
    uint8_t leastSignificantByte = memory[realAddress];
    uint8_t mostSignificantByte = memory[realAddress+1];
    uint16_t value = ((uint16_t)mostSignificantByte << 8) | leastSignificantByte;
    return value;
}

Address MemoryMapper::readAddressAt(Address address) {
    Address addressRead;
    addressRead.bank = readByte(address.bank, address.offset+2);
    addressRead.offset = readTwoBytes(address);
    return addressRead;
}

void logDecodedAddress(const char *type, uint8_t bank, uint16_t offset, uint8_t decodedBank, uint16_t decodedOffset) {
    /*Log &log = Log::dbg(LOG_TAG);
    log.str("Decoded address ").hex(bank, 2).str(":").hex(offset, 4);
    log.str(" to ").str(type).sp().hex(decodedBank, 2).str(":").hex(decodedOffset, 4);
    log.show();*/
}

uint8_t *MemoryMapper::decodeMemoryAddress(uint8_t bank, uint16_t offset, uint8_t *decodedBank, uint16_t *decodedOffset) {
    
    if ((bankInRange(bank, 0x00, 0x3F) || bankInRange(bank, 0x80, 0xBF) || bankIs(bank, 0x7E)) && 
         offsetInRange(offset, 0x0000, 0x1FFF)) {
             
        // LowRAM
        *decodedBank = 0x00;
        *decodedOffset = offset;
        logDecodedAddress("LowRAM", bank, offset, *decodedBank, *decodedOffset);
        
        return mLowRam;
        
    } else if ((bankInRange(bank, 0x00, 0x3F) || bankInRange(bank, 0x80, 0xBF)) && offsetIsInBankLow(offset)) {
        if (offsetInRange(offset, 0x2100, 0x21FF)) {

            // LowRAM
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

    } else if (bankIs(bank, 0x75) && offsetInRange(offset, 0x2000, 0x7FFF)) {
        
        *decodedBank = 0x00;
        *decodedOffset = offset - 0x2000;
        logDecodedAddress("HiRam", bank, offset, *decodedBank, *decodedOffset);
        
        return mHighRam;
        
    } else if (bankIs(bank, 0x75) && offsetIsInBankHigh(offset)) {
        
        *decodedBank = 0x00;
        *decodedOffset = offset - 0x8000;
        logDecodedAddress("ExpandedRam", bank, offset, *decodedBank, *decodedOffset);
        
        return mExpandedRam;
                
    } else if (bankIs(bank, 0x7F)) {
        
        if (offsetIsInBankLow(offset)) {
            *decodedBank = 0x00;
            *decodedOffset = offset + 0x8000;
        } else {
            *decodedBank = 0x01;
            *decodedOffset = offset - 0x8000;
        }
        
        logDecodedAddress("ExpandedRam", bank, offset, *decodedBank, *decodedOffset);
        
        return mExpandedRam;
        
    } else {
        
        *decodedBank = 0;
        *decodedOffset = 0;
        decodeRomAddress(bank, offset, decodedBank, decodedOffset);
        
        logDecodedAddress("ROM", bank, offset, *decodedBank, *decodedOffset);
        return mRomReader.getRomData();
    }
}

void MemoryMapper::decodeRomAddress(uint8_t bank, uint16_t offset, uint8_t *decodedBank, uint16_t *decodedOffset) {
    if (mRomReader.getRomType() == RomReader::LO_ROM) {
        // Mapping #0
        if (bankInRange(bank, 0x00, 0x3F) && offsetIsInBankHigh(offset)) {
            *decodedBank = std::floor(bank / 2);
            if (bankIsEven(bank)) *decodedOffset = offsetToBankLow(offset);
            else *decodedOffset = offset;
        } else if (bankInRange(bank, 0x80, 0xBF) && offsetIsInBankHigh(offset)) {
            bank -= 0x80;
            *decodedBank = std::floor(bank / 2);
            if (bankIsEven(bank)) *decodedOffset = offsetToBankLow(offset);
            else *decodedOffset = offset;
        } 
        // Mapping #1
        else if (bankInRange(bank, 0x40, 0x6F) && offsetIsInBankLow(offset)) {
            *decodedBank = std::floor(bank / 2);
            if (bankIsEven(bank)) *decodedOffset = offset;
            else *decodedOffset = offsetToBankHigh(offset);
        }
        // Mapping #2
        else if (bankInRange(bank, 0x40, 0x6F) && offsetIsInBankHigh(offset)) {
            *decodedBank = std::floor(bank / 2);
            if (bankIsEven(bank)) *decodedOffset = offsetToBankLow(offset);
            else *decodedOffset = offset;
        } else if (bankInRange(bank, 0xC0, 0xEF) && offsetIsInBankHigh(offset)) {
            bank -= 0x80;
            *decodedBank = std::floor(bank / 2);
            if (bankIsEven(bank)) *decodedOffset = offsetToBankLow(offset);
            else *decodedOffset = offset;
        }
        // Mapping #3
        else if (bankInRange(bank, 0x70, 0x7D) && offsetIsInBankHigh(offset)) {
            *decodedBank = std::floor(bank / 2);
            if (bankIsEven(bank)) *decodedOffset = offsetToBankLow(offset);
            else *decodedOffset = offset;
        } else if (bankInRange(bank, 0xF0, 0xFD) && offsetIsInBankHigh(offset)) {
            bank -= 0x80;
            *decodedBank = std::floor(bank / 2);
            if (bankIsEven(bank)) *decodedOffset = offsetToBankLow(offset);
            else *decodedOffset = offset;
        }
        // Mapping #4
        else if (bank == 0xFE && offsetIsInBankHigh(offset)) {
            *decodedBank = 0x3F;
            *decodedOffset = offsetToBankLow(offset);
        } else if (bank == 0xFF && offsetIsInBankHigh(offset)) {
            *decodedBank = 0x3F;
            *decodedOffset = offset;
        }
    } else if (mRomReader.getRomType() == RomReader::HI_ROM) {
        // Mapping #0
        if ((bank >= 0x00 && bank <= 0x1F) && offsetIsInBankHigh(offset)) {
            *decodedBank = bank;
            *decodedOffset = offset;
        } else if (bankInRange(bank, 0x80, 0x9F) && offsetIsInBankHigh(offset)) {
            bank -= 0x80;
            *decodedBank = bank;
            *decodedOffset = offset;
        }
        // Mapping #1
        if (bankInRange(bank, 0x20, 0x3F) && offsetIsInBankHigh(offset)) {
            *decodedBank = bank;
            *decodedOffset = offset;
        } else if (bankInRange(bank, 0xA0, 0xBF) && offsetIsInBankHigh(offset)) {
            bank -= 0x80;
            *decodedBank = bank;
            *decodedOffset = offset;
        }
        // Mapping #2
        if (bankInRange(bank, 0x40, 0x7D)) {
            bank -= 0x40;
            *decodedBank = bank;
            *decodedOffset = offset;
        } else if (bankInRange(bank, 0xC0, 0xFD)) {
            bank -= 0xC0;
            *decodedBank = bank;
            *decodedOffset = offset;
        }
        // Mapping #3
        if (bankInRange(bank, 0x70, 0x7D) && offsetIsInBankHigh(offset)) {
            *decodedBank = std::floor(bank / 2);
            if (bankIsEven(bank)) *decodedOffset = offsetToBankLow(offset);
            else *decodedOffset = offset;
        }
        // Mapping #4
        else if (bank == 0xFE) {
            *decodedBank = 0x3E;
            *decodedOffset = offset;
        } else if (bank == 0xFF) {
            *decodedBank = 0x3F;
            *decodedOffset = offset;
        }
    } else {
        Log::err(LOG_TAG).str("Error: unknown rom type, not HiRom nor LoRom").show();
    }
}

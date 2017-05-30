
#include <cmath>

#include "Cartridge.hpp"
#include "Log.hpp"

#define LOG_TAG "Cartridge"

/**
 * Reads the ROM into memory and initializes some pointers
 * to relevant structs inside the ROM.
 *
 * TODO: Handle ROMs with extra header.
 *
 */

Cartridge::Cartridge(const std::string &romFilePath) :
    mHeader(NULL), mRomData(NULL) {

    bool status = readROM(romFilePath);
	if (status) {
		// Ugly hack to be able to print the rom name, might sacrifice the last character.
		*(mHeader->romName + ROM_NAME_LENGTH_BYTES - 1) = 0;
		Log::dbg(LOG_TAG).str("Rom name is \"").str(mHeader->romName).str("\"").show();
	}
}

Cartridge::~Cartridge() {
    if (mRomData) delete mRomData;
}

RomType Cartridge::getRomType() {
	return mRomType;
}

uint8_t *Cartridge::getRomData() {
	return mRomData;
}

bool Cartridge::readROM(const std::string &romFilePath) {
	std::ifstream romFile(romFilePath.c_str(),
		std::ifstream::in | std::ifstream::binary | std::ifstream::ate);

    if (!romFile.is_open()) {
		Log::err(LOG_TAG).str("Cannot open rom file ").str(romFilePath.c_str()).show();
		return false;
    }

    // Find out the ROM total size
    mRomSizeBytes = romFile.tellg();

    Log::dbg(LOG_TAG).str("ROM size is").sp().dec(mRomSizeBytes).sp().str("bytes").sp();
    Log::dbg(LOG_TAG).str("(").dec(mRomSizeBytes / 1024).sp().str("kb)").show();

    // Load the whole ROM
    mRomData = new uint8_t[mRomSizeBytes];
    romFile.seekg (0, romFile.beg);
    romFile.read((char *)mRomData, mRomSizeBytes);
    romFile.close();

    // Time to read the ROM headers
    const uint8_t *romHeaderFromLo = mRomData + HEADER_LOCATION_LOROM;
    const uint8_t *romHeaderFromHi = mRomData + HEADER_LOCATION_HIROM;

    int loRomLocationReadableCharsCount = countReadableCharsInRomName((const char *)romHeaderFromLo);
    int hiRomLocationReadableCharsCount = countReadableCharsInRomName((const char *)romHeaderFromHi);

    if (loRomLocationReadableCharsCount > hiRomLocationReadableCharsCount) {
        Log::dbg(LOG_TAG).str("Looks like a LoRom").show();

        mRomType = LO_ROM;
        mHeader = (Header *) romHeaderFromLo;
    } else if (hiRomLocationReadableCharsCount > loRomLocationReadableCharsCount) {
        Log::dbg(LOG_TAG).str("Looks like a HiRom").show();

        mRomType = HI_ROM;
        mHeader = (Header *) romHeaderFromHi;
    } else {
		Log::dbg(LOG_TAG).str("Unsupported/Unknown Rom type!").show();
		return false;
	}

    return true;
}

uint8_t Cartridge::countReadableCharsInRomName(const char *romName) {
    const char *p = romName;
    const char *end = p + ROM_NAME_LENGTH_BYTES;
    int readableChars = 0;

    while (p < end) {
        if ( (*p >= 'A' && *p <= 'Z') ||
             (*p >= 'a' && *p <= 'z') ||
             (*p >= '0' && *p <= '9') ) {
             readableChars++;
        }
        p++;
    }

    return readableChars;
}

void Cartridge::storeByte(Address &address, uint8_t value) {
    // Writing to cartridge is not supported.
    Log::err(LOG_TAG).str("Error: trying to write to cartrige").show();
}

void Cartridge::storeTwoBytes(Address &address, uint16_t value) {
    // Writing to cartridge is not supported.
    Log::err(LOG_TAG).str("Error: trying to write to cartrige").show();
}

uint8_t Cartridge::readByte(Address address) {
    Address decodedAddress;
    decodeAddress(address, &decodedAddress);
    uint32_t realAddress = (decodedAddress.bank * BANK_SIZE_BYTES) + decodedAddress.offset;
    return mRomData[realAddress];
}

uint16_t Cartridge::readTwoBytes(Address address) {
    Address decodedAddress;
    decodeAddress(address, &decodedAddress);
    uint32_t realAddress = (decodedAddress.bank * BANK_SIZE_BYTES) + decodedAddress.offset;

    uint8_t leastSignificantByte = mRomData[realAddress];
    uint8_t mostSignificantByte = mRomData[realAddress+1];
    uint16_t value = ((uint16_t)mostSignificantByte << 8) | leastSignificantByte;
    return value;
}

Address Cartridge::readAddressAt(Address address) {
    Address addressOfBank;
    addressOfBank.bank = address.bank;
    addressOfBank.bank = address.offset + 2;

    Address addressRead;
    addressRead.bank = readByte(addressOfBank);
    addressRead.offset = readTwoBytes(address);
    return addressRead;
}

bool Cartridge::decodeAddress(Address virtualAddress, Address *decodedAddress) {
    uint8_t bank = virtualAddress.bank;
    uint16_t offset = virtualAddress.offset;

    if (mRomType == LO_ROM) {
        // Mapping #0
        if (bankInRange(bank, 0x00, 0x3F) && offsetIsInBankHigh(offset)) {
            decodedAddress->bank = std::floor(bank / 2);
            if (bankIsEven(bank)) decodedAddress->offset = offsetToBankLow(offset);
            else decodedAddress->offset = offset;
            return true;
        } else if (bankInRange(bank, 0x80, 0xBF) && offsetIsInBankHigh(offset)) {
            bank -= 0x80;
            decodedAddress->bank = std::floor(bank / 2);
            if (bankIsEven(bank)) decodedAddress->offset = offsetToBankLow(offset);
            else decodedAddress->offset = offset;
            return true;
        }
        // Mapping #1
        else if (bankInRange(bank, 0x40, 0x6F) && offsetIsInBankLow(offset)) {
            decodedAddress->bank = std::floor(bank / 2);
            if (bankIsEven(bank)) decodedAddress->offset = offset;
            else decodedAddress->offset = offsetToBankHigh(offset);
            return true;
        }
        // Mapping #2
        else if (bankInRange(bank, 0x40, 0x6F) && offsetIsInBankHigh(offset)) {
            decodedAddress->bank = std::floor(bank / 2);
            if (bankIsEven(bank)) decodedAddress->offset = offsetToBankLow(offset);
            else decodedAddress->offset = offset;
            return true;
        } else if (bankInRange(bank, 0xC0, 0xEF) && offsetIsInBankHigh(offset)) {
            bank -= 0x80;
            decodedAddress->bank = std::floor(bank / 2);
            if (bankIsEven(bank)) decodedAddress->offset = offsetToBankLow(offset);
            else decodedAddress->offset = offset;
            return true;
        }
        // Mapping #3
        else if (bankInRange(bank, 0x70, 0x7D) && offsetIsInBankHigh(offset)) {
            decodedAddress->bank = std::floor(bank / 2);
            if (bankIsEven(bank)) decodedAddress->offset = offsetToBankLow(offset);
            else decodedAddress->offset = offset;
            return true;
        } else if (bankInRange(bank, 0xF0, 0xFD) && offsetIsInBankHigh(offset)) {
            bank -= 0x80;
            decodedAddress->bank = std::floor(bank / 2);
            if (bankIsEven(bank)) decodedAddress->offset = offsetToBankLow(offset);
            else decodedAddress->offset = offset;
            return true;
        }
        // Mapping #4
        else if (bank == 0xFE && offsetIsInBankHigh(offset)) {
            decodedAddress->bank = 0x3F;
            decodedAddress->offset = offsetToBankLow(offset);
            return true;
        } else if (bank == 0xFF && offsetIsInBankHigh(offset)) {
            decodedAddress->bank = 0x3F;
            decodedAddress->offset = offset;
            return true;
        }
    } else if (getRomType() == HI_ROM) {
        // Mapping #0
        if ((bank >= 0x00 && bank <= 0x1F) && offsetIsInBankHigh(offset)) {
            decodedAddress->bank = bank;
            decodedAddress->offset = offset;
            return true;
        } else if (bankInRange(bank, 0x80, 0x9F) && offsetIsInBankHigh(offset)) {
            bank -= 0x80;
            decodedAddress->bank = bank;
            decodedAddress->offset = offset;
            return true;
        }
        // Mapping #1
        if (bankInRange(bank, 0x20, 0x3F) && offsetIsInBankHigh(offset)) {
            decodedAddress->bank = bank;
            decodedAddress->offset = offset;
            return true;
        } else if (bankInRange(bank, 0xA0, 0xBF) && offsetIsInBankHigh(offset)) {
            bank -= 0x80;
            decodedAddress->bank = bank;
            decodedAddress->offset = offset;
            return true;
        }
        // Mapping #2
        if (bankInRange(bank, 0x40, 0x7D)) {
            bank -= 0x40;
            decodedAddress->bank = bank;
            decodedAddress->offset = offset;
            return true;
        } else if (bankInRange(bank, 0xC0, 0xFD)) {
            bank -= 0xC0;
            decodedAddress->bank = bank;
            decodedAddress->offset = offset;
            return true;
        }
        // Mapping #3
        if (bankInRange(bank, 0x70, 0x7D) && offsetIsInBankHigh(offset)) {
            decodedAddress->bank = std::floor(bank / 2);
            if (bankIsEven(bank)) decodedAddress->offset = offsetToBankLow(offset);
            else decodedAddress->offset = offset;
            return true;
        }
        // Mapping #4
        else if (bank == 0xFE) {
            decodedAddress->bank = 0x3E;
            decodedAddress->offset = offset;
            return true;
        } else if (bank == 0xFF) {
            decodedAddress->bank = 0x3F;
            decodedAddress->offset = offset;
            return true;
        }
    } else {
        Log::err(LOG_TAG).str("Error: unknown rom type, not HiRom nor LoRom").show();
    }
    return false;
}

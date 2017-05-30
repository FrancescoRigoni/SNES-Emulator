
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

Cartridge::Cartridge(const std::string &romFilePath, MemoryMapper &memoryMapper) :
    mHeader(NULL), mRomData(NULL) {

    bool status = readROM(romFilePath);
	if (status) {
		// Ugly hack to be able to print the rom name, might sacrifice the last character.
		*(mHeader->romName + ROM_NAME_LENGTH_BYTES - 1) = 0;
		Log::dbg(LOG_TAG).str("Rom name is \"").str(mHeader->romName).str("\"").show();

		memoryMapper.registerDevice(this);
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

        mRomType = RomType::LO_ROM;
        mHeader = (Header *) romHeaderFromLo;
    } else if (hiRomLocationReadableCharsCount > loRomLocationReadableCharsCount) {
        Log::dbg(LOG_TAG).str("Looks like a HiRom").show();

        mRomType = RomType::HI_ROM;
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

void Cartridge::storeByte(const Address &address, uint8_t value) {
    // Writing to cartridge is not supported.
    Log::err(LOG_TAG).str("Error: trying to write to cartrige").show();
}

uint8_t Cartridge::readByte(const Address &address) {
    uint32_t realAddress = RAW_ADDRESS(address);
    return mRomData[realAddress];
}

bool Cartridge::decodeAddress(const Address &virtualAddress, Address &decodedAddress) {
    uint8_t bank = virtualAddress.getBank();
    uint16_t offset = virtualAddress.getOffset();

    uint8_t decodedBank;
    uint16_t decodedOffset;
    bool decoded = false;

    if (mRomType == RomType::LO_ROM) {
        // Mapping #0
        if (bankInRange(bank, 0x00, 0x3F) && offsetIsInBankHigh(offset)) {
            decodedBank = std::floor(bank / 2);
            if (bankIsEven(bank)) decodedOffset = offsetToBankLow(offset);
            else decodedOffset = offset;
            decoded = true;
        } else if (bankInRange(bank, 0x80, 0xBF) && offsetIsInBankHigh(offset)) {
            bank -= 0x80;
            decodedBank = std::floor(bank / 2);
            if (bankIsEven(bank)) decodedOffset = offsetToBankLow(offset);
            else decodedOffset = offset;
            decoded = true;
        }
        // Mapping #1
        else if (bankInRange(bank, 0x40, 0x6F) && offsetIsInBankLow(offset)) {
            decodedBank = std::floor(bank / 2);
            if (bankIsEven(bank)) decodedOffset = offset;
            else decodedOffset = offsetToBankHigh(offset);
            decoded = true;
        }
        // Mapping #2
        else if (bankInRange(bank, 0x40, 0x6F) && offsetIsInBankHigh(offset)) {
            decodedBank = std::floor(bank / 2);
            if (bankIsEven(bank)) decodedOffset = offsetToBankLow(offset);
            else decodedOffset = offset;
            decoded = true;
        } else if (bankInRange(bank, 0xC0, 0xEF) && offsetIsInBankHigh(offset)) {
            bank -= 0x80;
            decodedBank = std::floor(bank / 2);
            if (bankIsEven(bank)) decodedOffset = offsetToBankLow(offset);
            else decodedOffset = offset;
            decoded = true;
        }
        // Mapping #3
        else if (bankInRange(bank, 0x70, 0x7D) && offsetIsInBankHigh(offset)) {
            decodedBank = std::floor(bank / 2);
            if (bankIsEven(bank)) decodedOffset = offsetToBankLow(offset);
            else decodedOffset = offset;
            decoded = true;
        } else if (bankInRange(bank, 0xF0, 0xFD) && offsetIsInBankHigh(offset)) {
            bank -= 0x80;
            decodedBank = std::floor(bank / 2);
            if (bankIsEven(bank)) decodedOffset = offsetToBankLow(offset);
            else decodedOffset = offset;
            decoded = true;
        }
        // Mapping #4
        else if (bank == 0xFE && offsetIsInBankHigh(offset)) {
            decodedBank = 0x3F;
            decodedOffset = offsetToBankLow(offset);
            decoded = true;
        } else if (bank == 0xFF && offsetIsInBankHigh(offset)) {
            decodedBank = 0x3F;
            decodedOffset = offset;
            decoded = true;
        }
    } else if (getRomType() == RomType::HI_ROM) {
        // Mapping #0
        if ((bank >= 0x00 && bank <= 0x1F) && offsetIsInBankHigh(offset)) {
            decodedBank = bank;
            decodedOffset = offset;
            decoded = true;
        } else if (bankInRange(bank, 0x80, 0x9F) && offsetIsInBankHigh(offset)) {
            bank -= 0x80;
            decodedBank = bank;
            decodedOffset = offset;
            decoded = true;
        }
        // Mapping #1
        if (bankInRange(bank, 0x20, 0x3F) && offsetIsInBankHigh(offset)) {
            decodedBank = bank;
            decodedOffset = offset;
            decoded = true;
        } else if (bankInRange(bank, 0xA0, 0xBF) && offsetIsInBankHigh(offset)) {
            bank -= 0x80;
            decodedBank = bank;
            decodedOffset = offset;
            decoded = true;
        }
        // Mapping #2
        if (bankInRange(bank, 0x40, 0x7D)) {
            bank -= 0x40;
            decodedBank = bank;
            decodedOffset = offset;
            decoded = true;
        } else if (bankInRange(bank, 0xC0, 0xFD)) {
            bank -= 0xC0;
            decodedBank = bank;
            decodedOffset = offset;
            decoded = true;
        }
        // Mapping #3
        if (bankInRange(bank, 0x70, 0x7D) && offsetIsInBankHigh(offset)) {
            decodedBank = std::floor(bank / 2);
            if (bankIsEven(bank)) decodedOffset = offsetToBankLow(offset);
            else decodedOffset = offset;
            decoded = true;
        }
        // Mapping #4
        else if (bank == 0xFE) {
            decodedBank = 0x3E;
            decodedOffset = offset;
            decoded = true;
        } else if (bank == 0xFF) {
            decodedBank = 0x3F;
            decodedOffset = offset;
            decoded = true;
        }
    } else {
        Log::err(LOG_TAG).str("Error: unknown rom type, not HiRom nor LoRom").show();
    }

    if (decoded) {
        Address address(decodedBank, decodedOffset);
        decodedAddress = address;
    }

    return decoded;
}

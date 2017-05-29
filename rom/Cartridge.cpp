
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

uint8_t Cartridge::fetchByte(uint8_t bank, uint16_t offset) {
	Log::dbg(LOG_TAG).str("Fetching byte from").sp().hex(bank * BANK_SIZE_BYTES + offset, 6).show();
	return mRomData[bank * BANK_SIZE_BYTES + offset];
}

uint16_t Cartridge::fetchWord(uint8_t bank, uint16_t offset) {
	uint16_t mostSignificant = mRomData[bank * 0x10000 + offset+1];
	uint16_t leastSignificant = mRomData[bank * 0x10000 + offset];
	uint16_t value = (uint16_t)(mostSignificant << 8 | leastSignificant);
	Log::trc(LOG_TAG).str(":> ").hex(value, 4).show();
	return value;
}

uint32_t Cartridge::fetchDWord(uint8_t bank, uint16_t offset) {
	Log::dbg(LOG_TAG).str("RomReader::fetchDWord unimplemented!!").show();
}





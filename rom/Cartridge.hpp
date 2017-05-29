
#include <fstream>
#include <iostream>
#include <string>
#include <exception>
#include <stdint.h>

#include "Rom.hpp"
#include "utils.hpp"

#ifndef __CARTRIDGE__
#define __CARTRIDGE__

#define BANK_SIZE_BYTES 0x10000

class Cartridge {
    public:
        Cartridge(const std::string &);
        ~Cartridge();

        RomType getRomType();
        uint8_t *getRomData();

        uint8_t fetchByte(uint8_t , uint16_t);
        uint16_t fetchWord(uint8_t , uint16_t);
        uint32_t fetchDWord(uint8_t , uint16_t);
    private:
        uint32_t mRomSizeBytes;
        RomType mRomType;
        Header *mHeader;

        uint8_t *mRomData;

        bool readROM(const std::string &);
        uint8_t countReadableCharsInRomName(const char *);
};

#endif

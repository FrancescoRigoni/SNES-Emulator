
#ifndef __CARTRIDGE__
#define __CARTRIDGE__

#include <fstream>
#include <iostream>
#include <string>
#include <exception>
#include <stdint.h>

#include "Rom.hpp"
#include "Device.hpp"
#include "utils.hpp"

class Cartridge : public Device {
    public:
        Cartridge(const std::string &);
        ~Cartridge();

        RomType getRomType();
        uint8_t *getRomData();

        // Methods inherited from Device
        void storeByte(Address&, uint8_t);
        void storeTwoBytes(Address&, uint16_t);
        uint8_t readByte(Address);
        uint16_t readTwoBytes(Address);
        Address readAddressAt(Address);
        bool decodeAddress(Address, Address*);
        // End of methods inherited from Device

    private:

        uint32_t mRomSizeBytes;
        // TODO rom type should not be here
        RomType mRomType;
        Header *mHeader;
        uint8_t *mRomData;

        bool readROM(const std::string &);
        uint8_t countReadableCharsInRomName(const char *);
};

#endif

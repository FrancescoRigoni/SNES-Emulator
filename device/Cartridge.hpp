
#ifndef __CARTRIDGE__
#define __CARTRIDGE__

#include <fstream>
#include <iostream>
#include <string>
#include <exception>
#include <stdint.h>

#include "Rom.hpp"
#include "Device.hpp"
#include "MemoryMapper.hpp"
#include "utils.hpp"

class Cartridge : public Device {
    public:
        Cartridge(const std::string &, MemoryMapper &);
        ~Cartridge();

        RomType getRomType();
        uint8_t *getRomData();

        // Methods inherited from Device
        void storeByte(const Address &, uint8_t) override;
        uint8_t readByte(const Address &) override;
        bool decodeAddress(const Address &, Address &) override;
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

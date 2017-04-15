
#include <fstream>
#include <iostream>
#include <string>
#include <exception>
#include <stdint.h>
#include "utils.hpp"

#ifndef __ROMREADER__
#define __ROMREADER__

#define HEADER_LOCATION_LOROM 0x7FC0
#define HEADER_LOCATION_HIROM 0xFFC0

#define NATIVE_INTERRUPT_TABLE_LOROM 0x7FE4
#define NATIVE_INTERRUPT_TABLE_HIROM 0xFFE4

#define EMULATED_INTERRUPT_TABLE_LOROM 0x7FF4
#define EMULATED_INTERRUPT_TABLE_HIROM 0xFFF4

#define BANK_SIZE_BYTES 0x10000

#define ROM_NAME_LENGTH_BYTES 21

class RomReader {
    public:
        typedef enum {
            HI_ROM,
            LO_ROM
        } RomType;
    
        typedef struct {
            char romName[ROM_NAME_LENGTH_BYTES];
            const uint8_t romMakeUp;
            const uint8_t romType;
            const uint8_t romSize;
            const uint8_t sRamSize;
            const uint8_t destinationCode;
            const uint8_t fixedValue;
            const uint8_t version;
            const uint16_t complementCheck;
            const uint16_t checksum;
        } Header; 
    
        typedef struct {
            const uint16_t coProcessorEnable;
            const uint16_t brk;
            const uint16_t abort;
            const uint16_t nonMaskableInterrupt;
            const uint16_t reset;
            const uint16_t interrutRequest;
        } NativeModeInterrupts;
        
        typedef struct {
            const uint16_t coProcessorEnable;
            const uint16_t unused;
            const uint16_t abort;
            const uint16_t nonMaskableInterrupt;
            const uint16_t reset;
            const uint16_t brkIrq;
        } EmulationModeInterrupts;
    
        RomReader(const std::string &);
        ~RomReader();
        
        RomType getRomType();
        uint8_t *getRomData();
        
        // Temporary
        const NativeModeInterrupts *mNativeModeInterrupts;
        const EmulationModeInterrupts *mEmulationModeInterrupts;
        
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

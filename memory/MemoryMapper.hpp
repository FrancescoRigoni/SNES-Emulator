
#ifndef __MEMORYMAPPER__
#define __MEMORYMAPPER__

#include <stdint.h>
#include "RomReader.hpp"

#define LOWRAM_SIZE                     0x2000
#define PPU_APU_REGS_SIZE                0x200
#define DSP_SUPERFX_REGS_SIZE           0x1000
#define OLD_STYLE_JOYPAD_REGS_SIZE       0x100
#define DMA_PPU2_REGS_SIZE               0x300
#define CARTRIDGE_SRAM_SIZE             0x2000
#define HIGH_RAM_SIZE                   0x6000
#define EXPANDED_RAM_SIZE              0x18000

#define BANK_SIZE_BYTES                0x10000
#define PAGE_SIZE_BYTES                    256

typedef struct {
    uint8_t bank;
    uint16_t offset;
} Address;

class MemoryMapper {
    public:
        MemoryMapper(RomReader &);
        ~MemoryMapper();
        
        void storeByte(Address&, uint8_t);
        void storeTwoBytes(Address&, uint16_t);
        
        uint8_t readByte(Address);  
        uint16_t readTwoBytes(Address);
        Address readAddressAt(Address);
        
        uint8_t readByte(uint8_t, uint16_t);  
        uint16_t readTwoBytes(uint8_t, uint16_t);
        Address readAddressAt(uint8_t, uint16_t);
        
        static bool offsetsAreOnDifferentPages(uint16_t, uint16_t);
        
        static Address sumOffsetToAddressNoWrapAround(Address &, uint16_t);
        static Address sumOffsetToAddressWrapAround(Address &, uint16_t);
 
    private:
        uint8_t *decodeMemoryAddress(uint8_t, uint16_t, uint8_t *, uint16_t *);
        void decodeRomAddress(uint8_t, uint16_t, uint8_t *, uint16_t *);
    
        RomReader mRomReader;
    
        uint8_t *mLowRam;
        uint8_t *mPpuApuRegs;
        uint8_t *mDspSuperFxRegs;
        uint8_t *mOldStyleJoypadRegs;
        uint8_t *mDmaPpu2Regs;
        uint8_t *mCartridgeSRam;
        uint8_t *mHighRam;
        uint8_t *mExpandedRam;
};

#endif


#ifndef __MEMORYMAPPER__
#define __MEMORYMAPPER__

#include <stdint.h>
#include <vector>

#include "Device.hpp"

#define PPU_APU_REGS_SIZE                0x200
#define DSP_SUPERFX_REGS_SIZE           0x1000
#define OLD_STYLE_JOYPAD_REGS_SIZE       0x100
#define DMA_PPU2_REGS_SIZE               0x300
#define CARTRIDGE_SRAM_SIZE             0x2000

class MemoryMapper {
    public:
        MemoryMapper();
        ~MemoryMapper();

        void registerDevice(Device *);
        void storeByte(const Address&, uint8_t);
        void storeTwoBytes(const Address&, uint16_t);
        uint8_t readByte(const Address&);
        uint16_t readTwoBytes(const Address&);
        Address readAddressAt(const Address&);

    private:

        std::vector<Device *> mDevices;

        uint8_t *mPpuApuRegs;
        uint8_t *mDspSuperFxRegs;
        uint8_t *mOldStyleJoypadRegs;
        uint8_t *mDmaPpu2Regs;
        uint8_t *mCartridgeSRam;
};

#endif

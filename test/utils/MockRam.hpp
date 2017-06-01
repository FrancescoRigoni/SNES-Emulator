
#ifndef MOCK_RAM
#define MOCK_RAM

#include "Device.hpp"
#include "MemoryMapper.hpp"

class MockRam : public Device {
    public:
        MockRam(MemoryMapper &, uint32_t);
        ~MockRam();

        // Methods inherited from Device
        void storeByte(const Address &, uint8_t) override;
        uint8_t readByte(const Address &) override;
        bool decodeAddress(const Address &, Address &) override;
        // End of methods inherited from Device

        void copyDataBlock(uint8_t *, uint32_t, uint32_t);

    private:
        uint8_t *mRam;
};

#endif // MOCK_RAM

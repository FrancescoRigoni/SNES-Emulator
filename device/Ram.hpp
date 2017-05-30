#ifndef RAM_H
#define RAM_H

#include <Device.hpp>
#include <MemoryMapper.hpp>

#define LOWRAM_SIZE                     0x2000
#define HIGH_RAM_SIZE                   0x6000
#define EXPANDED_RAM_SIZE              0x18000

class Ram : public Device
{
    public:
        Ram(MemoryMapper &);
        ~Ram();

        // Methods inherited from Device
        void storeByte(const Address &, uint8_t) override;
        uint8_t readByte(const Address &) override;
        bool decodeAddress(const Address &, Address &) override;
        // End of methods inherited from Device

    private:
        uint8_t *mRam;
};

#endif // RAM_H

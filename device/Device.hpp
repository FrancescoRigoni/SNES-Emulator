#ifndef DEVICE_H
#define DEVICE_H

#include <stdint.h>

#define BANK_SIZE_BYTES                0x10000
#define HALF_BANK_SIZE_BYTES            0x8000
#define PAGE_SIZE_BYTES                    256

#define offsetIsInBankLow(offset) (offset < 0x8000)
#define offsetIsInBankHigh(offset) (offset >= 0x8000)

#define offsetToBankLow(offset) (offset - 0x8000)
#define offsetToBankHigh(offset) (offset + 0x8000)
#define bankIsEven(bank) (bank % 2 == 0)
#define bankInRange(bank, start, end) (bank >= start && bank <= end)
#define offsetInRange(offset, start, end) (offset >= start && offset <= end)
#define bankIs(bank, what) (bank == what)

#define RAW_ADDRESS(address) ((uint32_t)((address.getBank() * BANK_SIZE_BYTES) + address.getOffset()))

class Address {
    private:
        uint8_t mBank;
        uint16_t mOffset;

    public:
        static bool offsetsAreOnDifferentPages(uint16_t, uint16_t);
        static Address sumOffsetToAddress(const Address &, uint16_t);
        static Address sumOffsetToAddressNoWrapAround(const Address &, uint16_t);
        static Address sumOffsetToAddressWrapAround(const Address &, uint16_t);

        Address() {};
        Address(uint8_t bank, uint16_t offset) : mBank(bank), mOffset(offset) {};

        Address newWithOffset(uint16_t);
        Address newWithOffsetNoWrapAround(uint16_t);
        Address newWithOffsetWrapAround(uint16_t);

        void incrementBy(uint16_t);
        void decrementBy(uint16_t);

        void getBankAndOffset(uint8_t *bank, uint16_t *offset) {
            *bank = mBank;
            *offset = mOffset;
        }

        uint8_t getBank() const {
            return mBank;
        }

        uint16_t getOffset() const {
            return mOffset;
        }
};

/**
 Every device (PPU, APU, ...) implements this interface.
 */
class Device {
    public:
        virtual ~Device() {};

        /**
          Stores one byte to the real address represented by the specified virtual address.
          That is: maps the virtual address to the real one and stores one byte in it.
         */
        virtual void storeByte(const Address &, uint8_t) = 0;

        /**
          Reads one byte from the real address represented by the specified virtual address.
          That is: maps the virtual address to the real one and reads from it.
         */
        virtual uint8_t readByte(const Address &) = 0;

        /**
          Returns true if the address was decoded successfully by this device.
         */
        virtual bool decodeAddress(const Address &, Address &) = 0;
};

#endif // DEVICE_H

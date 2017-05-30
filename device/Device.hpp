#ifndef DEVICE_H
#define DEVICE_H

#define BANK_SIZE_BYTES                0x10000
#define PAGE_SIZE_BYTES                    256

#define offsetIsInBankLow(offset) (offset < 0x8000)
#define offsetIsInBankHigh(offset) (offset >= 0x8000)

#define offsetToBankLow(offset) (offset - 0x8000)
#define offsetToBankHigh(offset) (offset + 0x8000)
#define bankIsEven(bank) (bank % 2 == 0)
#define bankInRange(bank, start, end) (bank >= start && bank <= end)
#define offsetInRange(offset, start, end) (offset >= start && offset <= end)
#define bankIs(bank, what) (bank == what)

typedef struct {
    uint8_t bank;
    uint16_t offset;
} Address;

/**
 Every device (PPU, APU, ...) implements this interface.
 */
class Device {
    public:
        /**
          Stores one byte to the real address represented by the specified virtual address.
          That is: maps the virtual address to the real one and stores one byte in it.
         */
        virtual void storeByte(Address&, uint8_t) = 0;

        /**
          Stores one byte to the real address represented by the specified virtual address.
          That is: maps the virtual address to the real one and stores one byte in it.
         */
        virtual void storeTwoBytes(Address&, uint16_t) = 0;

        /**
          Reads one byte from the real address represented by the specified virtual address.
          That is: maps the virtual address to the real one and reads from it.
         */
        virtual uint8_t readByte(Address) = 0;

        /**
          Reads two bytes from the real address represented by the specified virtual address.
          That is: maps the virtual address to the real one and reads from it.
         */
        virtual uint16_t readTwoBytes(Address) = 0;

        /**
          Reads one Address from the real address represented by the specified virtual address.
          That is: maps the virtual address to the real one and reads one byte from it.
         */
        virtual Address readAddressAt(Address) = 0;

        /**
          Returns true if the address was decoded successfully by this device.
         */
        virtual bool decodeAddress(Address, Address*);
};

#endif // DEVICE_H

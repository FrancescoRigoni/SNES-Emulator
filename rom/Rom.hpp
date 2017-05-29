#ifndef ROM_HPP
#define ROM_HPP

#define HEADER_LOCATION_LOROM               0x7FC0
#define HEADER_LOCATION_HIROM               0xFFC0

#define NATIVE_INTERRUPT_TABLE_LOROM        0x7FE4
#define NATIVE_INTERRUPT_TABLE_HIROM        0xFFE4

#define EMULATED_INTERRUPT_TABLE_LOROM      0x7FF4
#define EMULATED_INTERRUPT_TABLE_HIROM      0xFFF4

#define ROM_NAME_LENGTH_BYTES                   21

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

#endif // ROM_HPP

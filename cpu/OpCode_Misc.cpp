#include "Cpu65816.hpp"

#define LOG_TAG "Cpu::executeMisc"

/**
 * This file contains implementations for all OpCodes that didn't fall into other categories.
 */

void Cpu65816::executeMisc(OpCode &opCode) {
    switch (opCode.getCode()) {
        case(0xEB):     // XBA
        {
            uint8_t lowerA = Binary::lower8BitsOf(mA);
            uint8_t higherA = Binary::higher8BitsOf(mA);
            mA = higherA | (((uint16_t)(lowerA)) << 8);
            mCpuStatus.updateSignAndZeroFlagFrom8BitValue(higherA);
            addToProgramAddressAndCycles(1, 3);
            break;
        }
        case(0x42):     // WDM
        {
            addToProgramAddress(2);
            addToCycles(2);
            // Check cycles should be equal to executing two NOPs
            break;
        }
        case(0x44):     // MVP
        {
            Address addressOfOpCodeData = getAddressOfOpCodeData(opCode);
            uint8_t destinationBank = mMemoryMapper.readByte(addressOfOpCodeData);
            addressOfOpCodeData.incrementOffsetBy(1);
            uint8_t sourceBank = mMemoryMapper.readByte(addressOfOpCodeData);

            Address sourceAddress(sourceBank, mX);
            Address destinationAddress(destinationBank, mY);

            while(mA != 0xFFFF) {
                uint8_t toTransfer = mMemoryMapper.readByte(sourceAddress);
                mMemoryMapper.storeByte(destinationAddress, toTransfer);

                sourceAddress.decrementOffsetBy(1);
                destinationAddress.decrementOffsetBy(1);
                mA--;

                addToCycles(7);
            }
            mDB = destinationBank;

            addToProgramAddress(3);
            break;
        }
        case(0x54):     // MVN
        {
            Address addressOfOpCodeData = getAddressOfOpCodeData(opCode);
            uint8_t destinationBank = mMemoryMapper.readByte(addressOfOpCodeData);
            addressOfOpCodeData.incrementOffsetBy(1);
            uint8_t sourceBank = mMemoryMapper.readByte(addressOfOpCodeData);

            Address sourceAddress(sourceBank, mX);
            Address destinationAddress(destinationBank, mY);

            while(mA != 0xFFFF) {
                uint8_t toTransfer = mMemoryMapper.readByte(sourceAddress);
                mMemoryMapper.storeByte(destinationAddress, toTransfer);

                sourceAddress.incrementOffsetBy(1);
                destinationAddress.incrementOffsetBy(1);
                mA--;

                addToCycles(7);
            }
            mDB = destinationBank;

            addToProgramAddress(3);
            break;
        }
        default:
        {
            LOG_UNEXPECTED_OPCODE(opCode);
        }
    }
}

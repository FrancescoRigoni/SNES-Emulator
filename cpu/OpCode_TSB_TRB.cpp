#include <Interrupt.hpp>
#include "Cpu65816.hpp"

#define LOG_TAG "Cpu::executeTSBTRB"

/**
 * This file contains the implementation for TSB and TRB OpCodes
 */

void Cpu65816::execute8BitTSB(OpCode &opCode) {
    const Address addressOfOpCodeData = getAddressOfOpCodeData(opCode);
    uint8_t value = mMemoryMapper.readByte(addressOfOpCodeData);
    uint8_t lowerA = Binary::lower8BitsOf(mA);
    const uint8_t result = value | lowerA;
    mMemoryMapper.storeByte(addressOfOpCodeData, result);

    if ((value & lowerA) == 0) mCpuStatus.setZeroFlag();
    else mCpuStatus.clearZeroFlag();
}

void Cpu65816::execute16BitTSB(OpCode &opCode) {
    const Address addressOfOpCodeData = getAddressOfOpCodeData(opCode);
    uint16_t value = mMemoryMapper.readTwoBytes(addressOfOpCodeData);
    const uint16_t result = value | mA;
    mMemoryMapper.storeTwoBytes(addressOfOpCodeData, result);

    if ((value & mA) == 0) mCpuStatus.setZeroFlag();
    else mCpuStatus.clearZeroFlag();
}

void Cpu65816::execute8BitTRB(OpCode &opCode) {
    const Address addressOfOpCodeData = getAddressOfOpCodeData(opCode);
    uint8_t value = mMemoryMapper.readByte(addressOfOpCodeData);
    uint8_t lowerA = Binary::lower8BitsOf(mA);
    const uint8_t result = value & ~lowerA;
    mMemoryMapper.storeByte(addressOfOpCodeData, result);

    if ((value & lowerA) == 0) mCpuStatus.setZeroFlag();
    else mCpuStatus.clearZeroFlag();
}

void Cpu65816::execute16BitTRB(OpCode &opCode) {
    const Address addressOfOpCodeData = getAddressOfOpCodeData(opCode);
    uint16_t value = mMemoryMapper.readTwoBytes(addressOfOpCodeData);
    const uint16_t result = value & ~mA;
    mMemoryMapper.storeTwoBytes(addressOfOpCodeData, result);

    if ((value & mA) == 0) mCpuStatus.setZeroFlag();
    else mCpuStatus.clearZeroFlag();
}

void Cpu65816::executeTSBTRB(OpCode &opCode) {
    switch (opCode.getCode()) {
        case(0x0C):                 // TSB Absolute
        {
            if (accumulatorIs8BitWide()) {
                execute8BitTSB(opCode);
            } else {
                execute16BitTSB(opCode);
                addToCycles(2);
            }
            addToProgramAddressAndCycles(3, 6);
            break;
        }
        case(0x04):                 // TSB Direct Page
        {
            if (accumulatorIs8BitWide()) {
                execute8BitTSB(opCode);
            } else {
                execute16BitTSB(opCode);
                addToCycles(2);
            }
            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }
            addToProgramAddressAndCycles(2, 5);
            break;
        }
        case(0x1C):                 // TRB Absolute
        {
            if (accumulatorIs8BitWide()) {
                execute8BitTRB(opCode);
            } else {
                execute16BitTRB(opCode);
                addToCycles(2);
            }
            addToProgramAddressAndCycles(3, 6);
            break;
        }
        case(0x14):                 // TRB Direct Page
        {
            if (accumulatorIs8BitWide()) {
                execute8BitTRB(opCode);
            } else {
                execute16BitTRB(opCode);
                addToCycles(2);
            }
            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }
            addToProgramAddressAndCycles(2, 5);
            break;
        }
        default: {
            LOG_UNEXPECTED_OPCODE(opCode);
        }
    }
}

#include "Cpu65816.hpp"

#define LOG_TAG "Cpu::executeSBC"

/**
 * This file contains the implementation for all SBC OpCodes.
 */

void Cpu65816::execute8BitSBC(OpCode &opCode) {
    Address dataAddress = getAddressOfOpCodeData(opCode);
    uint16_t value = (uint16_t) mMemoryMapper.readByte(dataAddress);
    uint16_t accumulator = (uint16_t) Binary::lower8BitsOf(mA);
    // If carry is clear we have to subtract one more
    if (!mCpuStatus.carryFlag()) {
        ++value;
    }
    // Let's use sum of two-complement instead of subtraction.
    //uint16_t twoComplementOfValue = ~value + 1;
    uint16_t result = (uint16_t)(accumulator + value);

    // Borrow was needed for this subtraction
    if (result & 0xFF00) mCpuStatus.clearCarryFlag();
    // Borrow was not needed for this subtraction
    else mCpuStatus.setCarryFlag();

    Binary::setLower8BitsOf16BitsValue(&mA, Binary::lower8BitsOf(result));
    mCpuStatus.updateSignAndZeroFlagFrom8BitValue(result);

}

void Cpu65816::execute16BitSBC(OpCode &opCode) {

}

void Cpu65816::execute8BitBCDSBC(OpCode &opCode) {

}

void Cpu65816::execute16BitBCDSBC(OpCode &opCode) {

}

void Cpu65816::executeSBC(OpCode &opCode) {
    switch (opCode.getCode()) {
        case(0x6B):                 // RTL
        {
            uint16_t offset = mStack.pull16Bit() + 1;
            uint8_t bank = mStack.pull8Bit();
            Address returnAddress(bank, offset);

            setProgramAddress(returnAddress);
            addToProgramAddressAndCycles(1,6);
            break;
        }
        default: {
            LOG_UNEXPECTED_OPCODE(opCode);
        }
    }
}

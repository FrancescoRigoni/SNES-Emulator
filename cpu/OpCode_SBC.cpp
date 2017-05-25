#include "Cpu.hpp"

#define LOG_TAG "Cpu::executeSBC"

/**
 * This file contains the implementation for all SBC OpCodes.
 */

void Cpu::execute8BitSBC(OpCode &opCode) {
    Address dataAddress = getAddressOfOpCodeData(opCode);
    uint16_t value = (uint16_t) mMemoryMapper.readByte(dataAddress);
    uint16_t accumulator = (uint16_t) Binary::lower8BitsOf(mA);
    // If carry is clear we have to subtract one more
    if (!mCpuStatus.carryFlag()) {
        ++value;
    }
    // Let's use sum of two-complement instead of subtraction.
    uint16_t twoComplementOfValue = ~value + 1;
    uint16_t result = (uint16_t)(accumulator + value);

    // Borrow was needed for this subtraction
    if (result & 0xFF00) mCpuStatus.clearCarryFlag();
    // Borrow was not needed for this subtraction
    else mCpuStatus.setCarryFlag();

    Binary::setLower8BitsOf16BitsValue(&mA, Binary::lower8BitsOf(result));
    mCpuStatus.updateSignAndZeroFlagFrom8BitValue(result);

}

void Cpu::execute16BitSBC(OpCode &opCode) {

}

void Cpu::execute8BitBCDSBC(OpCode &opCode) {

}

void Cpu::execute16BitBCDSBC(OpCode &opCode) {

}

void Cpu::executeSBC(OpCode &opCode) {
    switch (opCode.getCode()) {
        case(0x6B):                 // RTL
        {
            Address returnAddress;
            returnAddress.offset = mStack.pull16Bit() + 1;
            returnAddress.bank = mStack.pull8Bit();
            setProgramAddress(returnAddress);
            addToProgramAddressAndCycles(1,6);
            break;
        }
        default: {
            LOG_UNEXPECTED_OPCODE(opCode);
        }
    }
}

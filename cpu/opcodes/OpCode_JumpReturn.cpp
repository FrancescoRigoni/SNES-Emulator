#include "Cpu65816.hpp"

#define LOG_TAG "Cpu::executeJumpReturn"

/**
 * This file contains the implementation for all OpCodes
 * that deal with jumps, calls and returns.
 */

void Cpu65816::executeJumpReturn(OpCode &opCode) {
    switch (opCode.getCode()) {
        case(0x20):  // JSR Absolute
        {
            mStack.push16Bit(mProgramAddress.getOffset() + 2);
            uint16_t destinationAddress = getAddressOfOpCodeData(opCode).getOffset();
            setProgramAddress(Address(mProgramAddress.getBank(), destinationAddress));
            addToCycles(6);
            break;
        }
        case(0x22):  // JSR Absolute Long
        {
            mStack.push8Bit(mProgramAddress.getBank());
            mStack.push16Bit(mProgramAddress.getOffset() + 3);
            setProgramAddress(getAddressOfOpCodeData(opCode));
            addToCycles(8);
            break;
        }
        case(0xFC):  // JSR Absolute Indexed Indirect, X
        {
            Address destinationAddress = getAddressOfOpCodeData(opCode);
            mStack.push8Bit(mProgramAddress.getBank());
            mStack.push16Bit(mProgramAddress.getOffset() + 2);
            setProgramAddress(destinationAddress);
            addToCycles(8);
            break;
        }
        case(0x4C):  // JMP Absolute
        {
            uint16_t destinationAddress = getAddressOfOpCodeData(opCode).getOffset();
            setProgramAddress(Address(mProgramAddress.getBank(), destinationAddress));
            addToCycles(3);
            break;
        }
        case(0x6C):  // JMP Absolute Indirect
        {
            setProgramAddress(getAddressOfOpCodeData(opCode));
            addToCycles(5);
#ifdef EMU_65C02
            addToCycles(1);
#endif
            break;
        }
        case(0x7C):  // JMP Absolute Indexed Indirect, X
        {
            setProgramAddress(getAddressOfOpCodeData(opCode));
            addToCycles(6);
            break;
        }
        case(0x5C):  // JMP Absolute Long
        {
            setProgramAddress(getAddressOfOpCodeData(opCode));
            addToCycles(4);
            break;
        }
        case(0xDC):  // JMP Absolute Indirect Long
        {
            setProgramAddress(getAddressOfOpCodeData(opCode));
            addToCycles(6);
            break;
        }
        case(0x6B):                 // RTL
        {
            uint16_t newOffset = mStack.pull16Bit() + 1;
            uint8_t newBank = mStack.pull8Bit();

            Address returnAddress(newBank, newOffset);
            setProgramAddress(returnAddress);
            addToCycles(6);
            break;
        }
        case(0x60):                 // RTS
        {
            Address returnAddress(mProgramAddress.getBank(), mStack.pull16Bit() + 1);
            setProgramAddress(returnAddress);
            addToCycles(6);
            break;
        }
        default: {
            LOG_UNEXPECTED_OPCODE(opCode);
        }
    }
}

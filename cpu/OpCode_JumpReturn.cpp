#include "Cpu65816.hpp"

#define LOG_TAG "Cpu::executeJumpReturn"

/**
 * This file contains the implementation for all OpCodes
 * that deal with jumps, calls and returns.
 */

void Cpu65816::executeJumpReturn(OpCode &opCode) {
    switch (opCode.getCode()) {
        case(0x6B):                 // RTL
        {
            uint16_t newOffset = mStack.pull16Bit() + 1;
            uint8_t newBank = mStack.pull8Bit();

            Address returnAddress(newBank, newOffset);
            setProgramAddress(returnAddress);
            addToProgramAddressAndCycles(1,6);
            break;
        }
        case(0x60):                 // RTS
        {
            Address returnAddress(mProgramAddress.getBank(), mStack.pull16Bit());
            setProgramAddress(returnAddress);
            addToCycles(6);
            break;
        }
        default: {
            LOG_UNEXPECTED_OPCODE(opCode);
        }
    }
}

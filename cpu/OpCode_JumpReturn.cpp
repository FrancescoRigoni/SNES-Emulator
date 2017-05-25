#include "Cpu.hpp"

#define LOG_TAG "Cpu::executeJumpReturn"

/**
 * This file contains the implementation for all OpCodes
 * that deal with jumps, calls and returns.
 */

void Cpu::executeJumpReturn(OpCode &opCode) {
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
        case(0x60):                 // RTS
        {
            uint16_t offset = mStack.pull16Bit();
            setProgramAddress(mProgramAddress.bank, offset + 1);
            addToCycles(6);
            break;
        }
        default: {
            LOG_UNEXPECTED_OPCODE(opCode);
        }
    }
}

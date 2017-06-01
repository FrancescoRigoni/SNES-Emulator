#include "Cpu65816.hpp"

#define LOG_TAG "Cpu::executeSTX"

/**
 * This file contains the implementation for all STX OpCodes.
 */

void Cpu65816::executeSTX(OpCode &opCode) {
    Address dataAddress = getAddressOfOpCodeData(opCode);
    if (accumulatorIs8BitWide()) {
        mMemoryMapper.storeByte(dataAddress, Binary::lower8BitsOf(mX));
    } else {
        mMemoryMapper.storeTwoBytes(dataAddress, mX);
        addToCycles(1);
    }

    switch (opCode.getCode()) {
        case(0x8E):  // STX Absolute
        {
            addToProgramAddress(3);
            addToCycles(4);
            break;
        }
        case(0x86):  // STX Direct Page
        {
            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }

            addToProgramAddress(2);
            addToCycles(3);
            break;
        }
        case(0x96):  // STX Direct Page Indexed, Y
        {
            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }

            addToProgramAddress(2);
            addToCycles(4);
            break;
        }
        default: {
            LOG_UNEXPECTED_OPCODE(opCode);
        }
    }
}

#include "Cpu.hpp"

#define LOG_TAG "Cpu::executeSTZ"

/**
 * This file contains the implementation for all STZ OpCodes.
 */

void Cpu::executeSTZ(OpCode &opCode) {
    Address dataAddress = getAddressOfOpCodeData(opCode);
    if (accumulatorIs8BitWide()) {
        mMemoryMapper.storeByte(dataAddress, 0x00);
    } else {
        mMemoryMapper.storeTwoBytes(dataAddress, 0x0000);
        addToCycles(1);
    }

    switch (opCode.getCode()) {
        case(0x9C):  // STZ Absolute
        {
            addToProgramAddress(3);
            addToCycles(4);
            break;
        }
        case(0x64):  // STZ Direct Page
        {
            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }

            addToProgramAddress(2);
            addToCycles(3);
            break;
        }
        case(0x9E):  // STZ Absolute Indexed, X
        {
            addToProgramAddress(3);
            addToCycles(5);
            break;
        }
        case(0x74):  // STZ Direct Page Indexed, X
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

#include "Cpu65816.hpp"

#define LOG_TAG "Cpu::executeSTY"

/**
 * This file contains the implementation for all STY OpCodes.
 */

void Cpu65816::executeSTY(OpCode &opCode) {
    Address dataAddress = getAddressOfOpCodeData(opCode);
    if (accumulatorIs8BitWide()) {
        mMemoryMapper.storeByte(dataAddress, Binary::lower8BitsOf(mY));
    } else {
        mMemoryMapper.storeTwoBytes(dataAddress, mY);
        addToCycles(1);
    }

    switch (opCode.getCode()) {
        case(0x8C):  // STY Absolute
        {
            addToProgramAddress(3);
            addToCycles(4);
            break;
        }
        case(0x84):  // STY Direct Page
        {
            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }

            addToProgramAddress(2);
            addToCycles(3);
            break;
        }
        case(0x94):  // STY Direct Page Indexed, X
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

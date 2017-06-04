#include "Cpu65816.hpp"

#define LOG_TAG "Cpu::executeMisc"

/**
 * This file contains implementations for all OpCodes that didn't fall into other categories.
 */

void Cpu65816::executeMisc(OpCode &opCode) {
    switch (opCode.getCode()) {
        case(0xEB):  // XBA
        {
            uint8_t lowerA = Binary::lower8BitsOf(mA);
            uint8_t higherA = Binary::higher8BitsOf(mA);
            mA = higherA | (((uint16_t)(lowerA)) << 8);
            mCpuStatus.updateSignAndZeroFlagFrom8BitValue(higherA);
            addToProgramAddressAndCycles(1, 3);
            break;
        }
        default:
        {
            LOG_UNEXPECTED_OPCODE(opCode);
        }
    }
}

#include "Cpu.hpp"

#define LOG_TAG "Cpu::executeTAXTAY"

/**
 * This file contains the implementation for TAX and TAY OpCodes.
 */

void Cpu::executeTAXTAY(OpCode &opCode) {
    switch (opCode.getCode()) {
        case(0xA8):  // TAY
        {
            if ( (accumulatorIs8BitWide() && indexIs8BitWide()) ||
                 (accumulatorIs16BitWide() && indexIs8BitWide()) ) {
                uint8_t lower8BitsOfA = Binary::lower8BitsOf(mA);
                Binary::setLower8BitsOf16BitsValue(&mY, lower8BitsOfA);
                mCpuStatus.updateSignAndZeroFlagFrom8BitValue(lower8BitsOfA);
            } else {
                mY = mA;
                mCpuStatus.updateSignAndZeroFlagFrom16BitValue(mA);
            }
            addToProgramAddressAndCycles(1,2);
            break;
        }
        case(0xAA):  // TAX
        {
            if ( (accumulatorIs8BitWide() && indexIs8BitWide()) ||
                 (accumulatorIs16BitWide() && indexIs8BitWide()) ) {
                uint8_t lower8BitsOfA = Binary::lower8BitsOf(mA);
                Binary::setLower8BitsOf16BitsValue(&mX, lower8BitsOfA);
                mCpuStatus.updateSignAndZeroFlagFrom8BitValue(lower8BitsOfA);
            } else {
                mX = mA;
                mCpuStatus.updateSignAndZeroFlagFrom16BitValue(mA);
            }
            addToProgramAddressAndCycles(1,2);
            break;
        }
        default: {
            LOG_UNEXPECTED_OPCODE(opCode);
        }
    }
}

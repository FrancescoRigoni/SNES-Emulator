#include "Cpu.hpp"

#define LOG_TAG "Cpu::executeTDCTCD"

/**
 * This file contains the implementation for TDC and TCD OpCodes.
 */

void Cpu::executeTDCTCD(OpCode &opCode) {
    switch (opCode.getCode()) {
        case(0x5B):  // TCD
        {
            mD = mA;
            mCpuStatus.updateSignAndZeroFlagFrom16BitValue(mD);
            addToProgramAddressAndCycles(1, 2);
            break;
        }
        case(0x7B):  // TDC
        {
            mA = mD;
            mCpuStatus.updateSignAndZeroFlagFrom16BitValue(mA);
            addToProgramAddressAndCycles(1, 2);
            break;
        }
        default: {
            LOG_UNEXPECTED_OPCODE(opCode);
        }
    }
}

#include "Cpu.hpp"

#define LOG_TAG "Cpu::executeStatusReg"

/**
 * This file contains the implementation for all OpCodes
 * that deal directly with the status register.
 */

void Cpu::executeStatusReg(OpCode &opCode) {
    switch (opCode.getCode()) {
        case(0xC2):  // REP #const
        {
            uint8_t value = mMemoryMapper.readByte(getAddressOfOpCodeData(opCode));
            uint8_t statusByte = mCpuStatus.getRegisterValue();
            mCpuStatus.setRegisterValue(statusByte & ~value);
            addToProgramAddressAndCycles(2, 3);
            break;
        }
        default: {
            LOG_UNEXPECTED_OPCODE(opCode);
        }
    }
}

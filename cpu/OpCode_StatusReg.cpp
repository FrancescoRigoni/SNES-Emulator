#include "Cpu65816.hpp"

#define LOG_TAG "Cpu::executeStatusReg"

/**
 * This file contains the implementation for all OpCodes
 * that deal directly with the status register.
 */

void Cpu65816::executeStatusReg(OpCode &opCode) {
    switch (opCode.getCode()) {
        case(0xC2):  // REP #const
        {
            uint8_t value = mMemoryMapper.readByte(getAddressOfOpCodeData(opCode));
            uint8_t statusByte = mCpuStatus.getRegisterValue();
            mCpuStatus.setRegisterValue(statusByte & ~value);
            addToProgramAddressAndCycles(2, 3);
            break;
        }
        case(0x38):  // SEC
        {
            mCpuStatus.setCarryFlag();
            addToProgramAddressAndCycles(1, 2);
            break;
        }
        case(0xF8):  // SED
        {
            mCpuStatus.setDecimalFlag();
            addToProgramAddressAndCycles(1, 2);
            break;
        }
        case(0x78):  // SEI
        {
            mCpuStatus.setInterruptDisableFlag();
            addToProgramAddressAndCycles(1, 2);
            break;
        }
        case(0xE2):  // SEP
        {
            uint8_t value = mMemoryMapper.readByte(getAddressOfOpCodeData(opCode));
            if (mCpuStatus.emulationFlag()) {
                // In emulation mode status bits 4 and 5 are not affected
                // 0xCF = 11001111
                value &= 0xCF;
            }
            uint8_t statusReg = mCpuStatus.getRegisterValue();
            statusReg |= value;
            mCpuStatus.setRegisterValue(statusReg);

            addToProgramAddressAndCycles(2, 3);
            break;
        }
        default: {
            LOG_UNEXPECTED_OPCODE(opCode);
        }
    }
}

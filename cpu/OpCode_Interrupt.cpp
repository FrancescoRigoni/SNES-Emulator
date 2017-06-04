#include "Cpu65816.hpp"

#define LOG_TAG "Cpu::executeInterrupt"

/**
 * This file contains the implementation for all OpCodes
 * that deal with interrupts.
 */

void Cpu65816::executeInterrupt(OpCode &opCode) {
    switch (opCode.getCode()) {
        case(0x00):  // BRK
        {
            if (mCpuStatus.emulationFlag()) {
                mStack.push16Bit(mProgramAddress.getOffset()+2);
                mCpuStatus.setBreakFlag();
                mStack.push8Bit(mCpuStatus.getRegisterValue());
                mCpuStatus.setInterruptDisableFlag();
#ifdef EMU_65C02
                mCpuStatus.clearDecimalFlag();
#endif
                setProgramAddress(Address(0x00, mEmulationInterrupts->brkIrq));
                addToCycles(7);
            } else {
                mStack.push8Bit(mProgramAddress.getBank());
                mStack.push16Bit(mProgramAddress.getOffset()+2);
                mStack.push8Bit(mCpuStatus.getRegisterValue());
                mCpuStatus.setInterruptDisableFlag();
                mCpuStatus.clearDecimalFlag();
                Address newAddress(0x00, mNativeInterrupts->brk);
                setProgramAddress(newAddress);
                addToCycles(8);
            }
            break;
        }
        case(0x40):                 // RTI
        {
            // Note: The picture in the 65816 programming manual about this looks wrong.
            // This implementation follows the text instead.
            mCpuStatus.setRegisterValue(mStack.pull8Bit());

            if (mCpuStatus.emulationFlag()) {
                Address newProgramAddress(mProgramAddress.getBank(), mStack.pull16Bit());
                mProgramAddress = newProgramAddress;
                addToCycles(6);
            } else {
                uint16_t offset = mStack.pull16Bit();
                uint8_t bank = mStack.pull8Bit();
                Address newProgramAddress(bank, offset);
                mProgramAddress = newProgramAddress;
                addToCycles(7);
            }
        }
        default: {
            LOG_UNEXPECTED_OPCODE(opCode);
        }
    }
}

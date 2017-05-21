
#include "Cpu.hpp"

#include <cmath>

/**
 * This file contains a bunch of methods used by branch OpCodes.
 * Actual execution of the opcodes is performed inside Executor.cpp
 * These methods are called from there.
 */
int Cpu::executeBranchShortOnCondition(bool condition, OpCode &opCode) {
    uint8_t opCycles = 2;
    uint8_t destination =  mMemoryMapper.readByte(getAddressOfOpCodeData(opCode));
    // This is the address of the next instruction
    uint16_t actualDestination;
    if (condition) {
        // One extra cycle if the branch is taken
        opCycles++;
        uint16_t destination16;
        if (Binary::is8bitValueNegative(destination)) {
            destination16 = 0xFF00 | destination;
        } else {
            destination16 = destination;
        }
        actualDestination = mProgramAddress.offset + 2 + destination16;
        // Emulation mode requires 1 extra cycle on page boundary crossing
        if (MemoryMapper::offsetsAreOnDifferentPages(mProgramAddress.offset, actualDestination) &&
            mCpuStatus.emulationFlag()) {
            opCycles++;
        }
    } else {
        actualDestination = mProgramAddress.offset + 2;
    }
    mProgramAddress.offset = actualDestination;
    return opCycles;
}

int Cpu::executeBranchLongOnCondition(bool condition, OpCode &opCode) {
    if (condition) {
        uint16_t destination = mMemoryMapper.readTwoBytes(getAddressOfOpCodeData(opCode));
        mProgramAddress.offset += 3 + destination;
    }
    // CPU cycles: 4
    return 4;
}
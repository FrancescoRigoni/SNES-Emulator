
#include "Cpu65816.hpp"

#include <cmath>

#ifdef EMU_65C02
#define LOG_TAG "Cpu65C02"
#else
#define LOG_TAG (mCpuStatus.emulationFlag() ? "Cpu6502" : "Cpu65816")
#endif

Cpu65816::Cpu65816(MemoryMapper &memoryMapper, EmulationModeInterrupts *emulationInterrupts, NativeModeInterrupts *nativeInterrupts) :
            mMemoryMapper(memoryMapper),
            mEmulationInterrupts(emulationInterrupts),
            mNativeInterrupts(nativeInterrupts),
            mStack(&mMemoryMapper) {
}

/**
 * Resets the cpu to its initial state.
 * */
void Cpu65816::reset() {
    setRESPin(true);
    mCpuStatus.setEmulationFlag();
    mCpuStatus.setAccumulatorWidthFlag();
    mCpuStatus.setIndexWidthFlag();
    mX &= 0xFF;
    mY &= 0xFF;
    mD = 0x0;
    mStack = Stack(&mMemoryMapper);
    mProgramAddress = Address(0x00, mEmulationInterrupts->reset);
}

void Cpu65816::setRESPin(bool value) {
    if (value == false && mPins.RES == true) {
        reset();
    }
    mPins.RES = value;
}

void Cpu65816::setRDYPin(bool value) {
    mPins.RDY = value;
}

bool Cpu65816::executeNextInstruction() {
    if (mPins.RES) {
        return false;
    }

    // Fetch the instruction
    const uint8_t instruction = mMemoryMapper.readByte(mProgramAddress);
    OpCode opCode = OP_CODE_TABLE[instruction];

    // Execute it
    return opCode.execute(*this);
}

bool Cpu65816::accumulatorIs8BitWide() {
    // Accumulator is always 8 bit in emulation mode.
    if (mCpuStatus.emulationFlag()) return true;
    // Accumulator width set to one means 8 bit accumulator.
    else if (mCpuStatus.accumulatorWidthFlag()) return true;
    // Else it is 16 bit wide.
    else return false;
}

bool Cpu65816::accumulatorIs16BitWide() {
    return !accumulatorIs8BitWide();
}

bool Cpu65816::indexIs8BitWide() {
    // Index is always 8 bit in emulation mode.
    if (mCpuStatus.emulationFlag()) return true;
    // Index width set to one means 8 bit accumulator.
    else if (mCpuStatus.indexWidthFlag()) return true;
    // Else it is 16 bit wide.
    else return false;
}

bool Cpu65816::indexIs16BitWide() {
    return !indexIs8BitWide();
}

void Cpu65816::addToCycles(int cycles) {
    mTotalCyclesCounter += cycles;
}

void Cpu65816::subtractFromCycles(int cycles) {
    mTotalCyclesCounter -= cycles;
}

void Cpu65816::addToProgramAddress(int bytes) {
    mProgramAddress.incrementOffsetBy(bytes);
}

void Cpu65816::addToProgramAddressAndCycles(int bytes, int cycles) {
    addToCycles(cycles);
    addToProgramAddress(bytes);
}

uint16_t Cpu65816::indexWithXRegister() {
    return indexIs8BitWide() ? Binary::lower8BitsOf(mX) : mX;
}

uint16_t Cpu65816::indexWithYRegister() {
    return indexIs8BitWide() ? Binary::lower8BitsOf(mY) : mY;
}

void Cpu65816::setProgramAddress(const Address &address) {
    mProgramAddress = address;
}

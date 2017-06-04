
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
            mStack(&mMemoryMapper),
            mProgramAddress(0x00, emulationInterrupts->reset) {

    // Start in emulation mode
    mCpuStatus.setEmulationFlag();

    logCpuStatus();

    Log::dbg(LOG_TAG).str("Emulation mode RST vector at").sp().hex(mEmulationInterrupts->reset, 4).show();
    Log::dbg(LOG_TAG).str("Native mode BRK vector at").sp().hex(mNativeInterrupts->brk, 4).show();
    Log::dbg(LOG_TAG).str("Native mode VSYNC vector at").sp().hex(mNativeInterrupts->nonMaskableInterrupt, 4).show();
}

void Cpu65816::debug_setZeroFlag() {
    Log::dbg(LOG_TAG).str(">> Forcing zero flag to 1").show();
    mCpuStatus.setZeroFlag();
}

void Cpu65816::setBreakPoint(uint8_t bank, uint16_t offset) {
    mBreakpointEnabled = true;
    mBreakBank = bank;
    mBreakOffset = offset;
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
    mProgramAddress.incrementBy(bytes);
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

bool Cpu65816::executeNext() {
    // Fetch the instruction
    const uint8_t instruction = mMemoryMapper.readByte(mProgramAddress);
    OpCode opCode = OP_CODE_TABLE[instruction];
    // Log the OpCode
    trace(opCode);
    // TODO: make decent debugger
    if (mBreakpointEnabled) {
        if (mProgramAddress.getBank() == mBreakBank && mProgramAddress.getOffset() == mBreakOffset) return false;
    }
    // Execute it
    return opCode.execute(*this);
}

void Cpu65816::logCpuStatus() {
    Log::trc(LOG_TAG).str("====== CPU status start ======").show();
    Log::trc(LOG_TAG).str("A: ").hex(mA, 4).sp().str("X: ").hex(mX, 4).sp().str("Y: ").hex(mY, 4).show();
    Log::trc(LOG_TAG).str("PB: ").hex(mProgramAddress.getBank(), 2).sp().str("PC: ").hex(mProgramAddress.getOffset(), 4).show();
    Log::trc(LOG_TAG).str("DB: ").hex(mDB, 2).sp().str("D: ").hex(mD, 4).show();
    Log::trc(LOG_TAG).str("S (Stack pointer): ").hex(mStack.getStackPointer(), 4).show();
    Log::trc(LOG_TAG).str("P (Status): ").hex(mCpuStatus.getRegisterValue(), 2).show();
    Log::trc(LOG_TAG).str("Negative: ").str(mCpuStatus.signFlag() ? "1" : "0").show();
    Log::trc(LOG_TAG).str("Zero: ").str(mCpuStatus.zeroFlag() ? "1" : "0").show();
    Log::trc(LOG_TAG).str("====== CPU status end ======").show();
}

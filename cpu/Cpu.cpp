
#include "Cpu.hpp"

#include <cmath>

#define LOG_TAG "Cpu"

Cpu::Cpu(MemoryMapper &memoryMapper, EmulationModeInterrupts *emulationInterrupts, NativeModeInterrupts *nativeInterrupts) :
            mMemoryMapper(memoryMapper),
            mEmulationInterrupts(emulationInterrupts),
            mNativeInterrupts(nativeInterrupts),
            mStack(&mMemoryMapper),
            mProgramAddress(0x00, emulationInterrupts->reset) {

    initCpu();
    logCpuStatus();

    Log::dbg(LOG_TAG).str("Emulation mode RST vector at").sp().hex(mEmulationInterrupts->reset, 4).show();
    Log::dbg(LOG_TAG).str("Native mode BRK vector at").sp().hex(mNativeInterrupts->brk, 4).show();
    Log::dbg(LOG_TAG).str("Native mode VSYNC vector at").sp().hex(mNativeInterrupts->nonMaskableInterrupt, 4).show();
}

void Cpu::debug_setZeroFlag() {
    Log::dbg(LOG_TAG).str(">> Forcing zero flag to 1").show();
    mCpuStatus.setZeroFlag();
}

void Cpu::initCpu() {
    mBreakpointEnabled = false;

    mA = 0;
    mX = 0;
    mY = 0;
    mDB = 0;
    mD = 0;

    mTotalCyclesCounter = 0;

    // Start in emulation mode
    mCpuStatus.setEmulationFlag();
}

void Cpu::setBreakPoint(uint8_t bank, uint16_t offset) {
    mBreakpointEnabled = true;
    mBreakBank = bank;
    mBreakOffset = offset;
}

bool Cpu::accumulatorIs8BitWide() {
    return mCpuStatus.accumulatorWidthFlag() || mCpuStatus.emulationFlag();
}

bool Cpu::accumulatorIs16BitWide() {
    return !accumulatorIs8BitWide();
}

bool Cpu::indexIs8BitWide() {
    return mCpuStatus.indexWidthFlag() || mCpuStatus.emulationFlag();
}

bool Cpu::indexIs16BitWide() {
    return !indexIs8BitWide();
}

void Cpu::addToCycles(int cycles) {
    mTotalCyclesCounter += cycles;
}

void Cpu::subtractFromCycles(int cycles) {
    mTotalCyclesCounter -= cycles;
}

void Cpu::addToProgramAddress(int bytes) {
    mProgramAddress.incrementBy(bytes);
}

void Cpu::addToProgramAddressAndCycles(int bytes, int cycles) {
    addToCycles(cycles);
    addToProgramAddress(bytes);
}

uint16_t Cpu::indexWithXRegister() {
    return indexIs8BitWide() ? Binary::lower8BitsOf(mX) : mX;
}

uint16_t Cpu::indexWithYRegister() {
    return indexIs8BitWide() ? Binary::lower8BitsOf(mY) : mY;
}

void Cpu::setProgramAddress(const Address &address) {
    mProgramAddress = address;
}

void Cpu::logCpuStatus() {
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


#include <stdint.h>

#include "MemoryMapper.hpp"
#include "RomReader.hpp"
#include "Stack.hpp"
#include "CpuStatus.hpp"
#include "Log.hpp"

#ifndef __CPU__
#define __CPU__

enum AddressingMode {
    Interrupt,
    Accumulator,
    BlockMove,
    Implied,
    Immediate,
    Absolute,
    AbsoluteProgram,         // Absolute using Program Bank instead of Data Bank
    AbsoluteLong,
    AbsoluteIndirect,
    AbsoluteIndirectLong,
    AbsoluteIndexedIndirect, // index with X
    AbsoluteIndexedWithX,
    AbsoluteLongIndexedWithX,
    AbsoluteIndexedWithY,
    DirectPage,
    DirectPageIndexedWithX,
    DirectPageIndexedWithY,
    DirectPageIndirect,
    DirectPageIndirectLong,
    DirectPageIndexedIndirectWithX,
    DirectPageIndirectIndexedWithY,
    DirectPageIndirectLongIndexedWithY,
    StackImplied,
    StackRelative,
    StackAbsolute,
    StackDirectPageIndirect,
    StackProgramCounterRelativeLong,
    StackRelativeIndirectIndexedWithY,
    ProgramCounterRelative,
    ProgramCounterRelativeLong
};

class OpCode {
    private:
        uint8_t mCode;
        const char * const mName;
        AddressingMode mAddressingMode;
    public:
        OpCode(uint8_t, const char * const, const AddressingMode &);

        uint8_t getCode() {
            return mCode;
        }

        const char *getName() {
            return mName;
        }

        const AddressingMode getAddressingMode() {
            return mAddressingMode;
        }
};

class Cpu {
    public:
        Cpu(RomReader &, MemoryMapper &);

        void setBreakPoint(uint8_t bank, uint16_t offset);

        // Temporary
        bool executeNext();

        void logCpuStatus();

        void debug_setZeroFlag();

    private:
        const RomReader::EmulationModeInterrupts * const mEmulationInterrupts;
        const RomReader::NativeModeInterrupts * const mNativeInterrupts;
        MemoryMapper &mMemoryMapper;

        // Accumulator register
        uint16_t mA;
        // X index register
        uint16_t mX;
        // Y ndex register
        uint16_t mY;
        // Status register
        CpuStatus mCpuStatus;
        // Program bank register
        //uint8_t mPB;
        // Data bank rgister
        uint8_t mDB;
        // Program counter register
        //uint16_t mPC;
        // Direct page register
        uint16_t mD;

        Stack mStack;

        // Address of the current OpCode
        Address mProgramAddress;

        // Total number of cycles
        uint64_t mTotalCyclesCounter;

        bool mBreakpointEnabled;
        uint8_t mBreakBank;
        uint16_t mBreakOffset;

        void initCpu();
        bool accumulatorIs8BitWide();
        bool accumulatorIs16BitWide();
        bool indexIs8BitWide();
        bool indexIs16BitWide();

        uint16_t indexWithXRegister();
        uint16_t indexWithYRegister();

        int executeBranchShortOnCondition(bool, OpCode &);
        int executeBranchLongOnCondition(bool, OpCode &);

        Address getAddressOfOpCodeData(OpCode &opCode);
        void trace(OpCode &opCode);

        void setProgramAddress(uint8_t, uint16_t);
        void setProgramAddress(Address &);
        void addToCycles(int);
        void addToProgramAddress(int);
        void addToProgramAddressAndCycles(int, int);
};

#endif

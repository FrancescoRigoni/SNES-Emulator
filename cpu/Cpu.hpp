
#include <stdint.h>

#include "MemoryMapper.hpp"
#include "RomReader.hpp"
#include "Addressing.hpp"
#include "Stack.hpp"
#include "CpuStatus.hpp"
#include "OpCode.hpp"
#include "Log.hpp"

#ifndef __CPU__
#define __CPU__

// Uncomment the following define for 65C02 emulation
// #define EMU_65C02

#define LOG_UNEXPECTED_OPCODE(opCode) Log::err(LOG_TAG).str("Unexpected OpCode: ").str(opCode.getName()).show();

class Cpu {
    public:
        Cpu(RomReader &, MemoryMapper &);

        void setBreakPoint(uint8_t bank, uint16_t offset);

        // Temporary
        bool executeNext();
        // Deprecated
        bool useDeprecatedExecutor(OpCode &opCode);

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

        Address getAddressOfOpCodeData(OpCode &);
        bool opCodeAddressingCrossesPageBoundary(OpCode &);

        void trace(OpCode &);

        void setProgramAddress(uint8_t, uint16_t);
        void setProgramAddress(Address &);
        void addToCycles(int);
        void addToProgramAddress(int);
        void addToProgramAddressAndCycles(int, int);

        int executeBranchShortOnCondition(bool, OpCode &);
        int executeBranchLongOnCondition(bool, OpCode &);

        // OpCodes implementations. See OpCode_XXX.
        void executeORA(OpCode &);
        void executeORA8Bit(OpCode &);
        void executeORA16Bit(OpCode &);
        void executeStack(OpCode &);
        void executeStatusReg(OpCode &);
        void executeMemoryROL(OpCode &);
        void executeAccumulatorROL(OpCode &);
        void executeROL(OpCode &);
        void executeMemoryROR(OpCode &);
        void executeAccumulatorROR(OpCode &);
        void executeROR(OpCode &);
        void executeInterrupt(OpCode &);
        void executeJumpReturn(OpCode &);
        void execute8BitSBC(OpCode &);
        void execute16BitSBC(OpCode &);
        void execute8BitBCDSBC(OpCode &);
        void execute16BitBCDSBC(OpCode &);
        void executeSBC(OpCode &);
        void execute8BitADC(OpCode &);
        void execute16BitADC(OpCode &);
        void execute8BitBCDADC(OpCode &);
        void execute16BitBCDADC(OpCode &);
        void executeADC(OpCode &);

        static OpCode OP_CODE_TABLE[];
};

#endif

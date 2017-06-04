
#include <stdint.h>

#include "MemoryMapper.hpp"
#include "Cartridge.hpp"
#include "Interrupt.hpp"
#include "Addressing.hpp"
#include "Stack.hpp"
#include "CpuStatus.hpp"
#include "OpCode.hpp"
#include "Log.hpp"

#ifndef __CPU_65816__
#define __CPU_65816__

// Uncomment the following define for 65C02 emulation
// #define EMU_65C02

#define LOG_UNEXPECTED_OPCODE(opCode) Log::err(LOG_TAG).str("Unexpected OpCode: ").str(opCode.getName()).show();

class Cpu65816 {
    public:
        Cpu65816(MemoryMapper &, EmulationModeInterrupts *, NativeModeInterrupts *);

        // Temporary
        bool executeNext();
        // Deprecated
        bool useDeprecatedExecutor(OpCode &);

        // Debug stuff
        // TODO: Move away
        void logCpuStatus();
        bool mBreakpointEnabled = false;
        uint8_t mBreakBank;
        uint16_t mBreakOffset;

        void debug_setZeroFlag();
        void setBreakPoint(uint8_t, uint16_t);

        void trace(OpCode &);
        // End of debug stuff

    private:
        MemoryMapper &mMemoryMapper;

        EmulationModeInterrupts *mEmulationInterrupts;
        NativeModeInterrupts *mNativeInterrupts;

        // Accumulator register
        uint16_t mA = 0;
        // X index register
        uint16_t mX = 0;
        // Y index register
        uint16_t mY = 0;
        // Status register
        CpuStatus mCpuStatus;
        // Data bank rgister
        uint8_t mDB = 0;
        // Direct page register
        uint16_t mD = 0;

        Stack mStack;

        // Address of the current OpCode
        Address mProgramAddress {0x00, 0x0000};

        // Total number of cycles
        uint64_t mTotalCyclesCounter = 0;

        bool accumulatorIs8BitWide();
        bool accumulatorIs16BitWide();
        bool indexIs8BitWide();
        bool indexIs16BitWide();

        uint16_t indexWithXRegister();
        uint16_t indexWithYRegister();

        Address getAddressOfOpCodeData(OpCode &);
        bool opCodeAddressingCrossesPageBoundary(OpCode &);

        void setProgramAddress(const Address &);
        void addToCycles(int);
        void subtractFromCycles(int);
        void addToProgramAddress(int);
        void addToProgramAddressAndCycles(int, int);

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
        void executeSTA(OpCode &);
        void executeSTX(OpCode &);
        void executeSTY(OpCode &);
        void executeSTZ(OpCode &);
        void executeTransfer(OpCode &);
        void executeMemoryASL(OpCode &);
        void executeAccumulatorASL(OpCode &);
        void executeASL(OpCode &);
        void executeAND8Bit(OpCode &);
        void executeAND16Bit(OpCode &);
        void executeAND(OpCode &);
        void executeLDA8Bit(OpCode &);
        void executeLDA16Bit(OpCode &);
        void executeLDA(OpCode &);
        void executeLDX8Bit(OpCode &);
        void executeLDX16Bit(OpCode &);
        void executeLDX(OpCode &);
        void executeLDY8Bit(OpCode &);
        void executeLDY16Bit(OpCode &);
        void executeLDY(OpCode &);
        void executeEOR8Bit(OpCode &);
        void executeEOR16Bit(OpCode &);
        void executeEOR(OpCode &);
        int executeBranchShortOnCondition(bool, OpCode &);
        int executeBranchLongOnCondition(bool, OpCode &);
        void executeBranch(OpCode &);
        void execute8BitCMP(OpCode &);
        void execute16BitCMP(OpCode &);
        void executeCMP(OpCode &);
        void execute8BitDecInMemory(OpCode &);
        void execute16BitDecInMemory(OpCode &);
        void execute8BitIncInMemory(OpCode &);
        void execute16BitIncInMemory(OpCode &);
        void executeINCDEC(OpCode &);


        static OpCode OP_CODE_TABLE[];
};

#endif

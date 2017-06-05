
#ifndef __CPU_65816__
#define __CPU_65816__

#include <stdint.h>

#include "MemoryMapper.hpp"
#include "Cartridge.hpp"
#include "Interrupt.hpp"
#include "Addressing.hpp"
#include "Stack.hpp"
#include "CpuStatus.hpp"
#include "OpCode.hpp"
#include "Log.hpp"
#include "BuildConfig.hpp"

// Macro used by OpCode methods when an unrecognized OpCode is being executed.
#define LOG_UNEXPECTED_OPCODE(opCode) Log::err(LOG_TAG).str("Unexpected OpCode: ").str(opCode.getName()).show();

class Cpu65816Debugger;

class Cpu65816 {
        friend class Cpu65816Debugger;
    public:
        Cpu65816(MemoryMapper &, EmulationModeInterrupts *, NativeModeInterrupts *);

        void setRESPin(bool);
        void setRDYPin(bool);

        // Temporary
        bool executeNextInstruction();

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
        // Data bank register
        uint8_t mDB = 0;
        // Direct page register
        uint16_t mD = 0;

        struct {
            // Reset to true means low power mode (do nothing)
            bool RES = true;
            // Ready to false means CPU is waiting for an NMI/IRQ/ABORT/RESET
            bool RDY = false;
        } mPins;

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

        // OpCode Table.
        static OpCode OP_CODE_TABLE[];

        // OpCodes handling routines.
        // Implementations for these methods can be found in the corresponding OpCode_XXX.cpp file.
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
        void execute8BitCPX(OpCode &);
        void execute16BitCPX(OpCode &);
        void execute8BitCPY(OpCode &);
        void execute16BitCPY(OpCode &);
        void executeCPXCPY(OpCode &);
        void execute8BitTSB(OpCode &);
        void execute16BitTSB(OpCode &);
        void execute8BitTRB(OpCode &);
        void execute16BitTRB(OpCode &);
        void executeTSBTRB(OpCode &);
        void execute8BitBIT(OpCode &);
        void execute16BitBIT(OpCode &);
        void executeBIT(OpCode &);
        void executeMemoryLSR(OpCode &);
        void executeAccumulatorLSR(OpCode &);
        void executeLSR(OpCode &);
        void executeMisc(OpCode &);

        void reset();
};

#endif

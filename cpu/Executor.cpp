
#include "Cpu.hpp"

#include <cmath>

#define LOG_TAG "Cpu"

// OpCode table. See OpCode.cpp
extern OpCode OP_CODE[];

bool Cpu::executeNext() {
    // Fetch the instruction
    const uint8_t instruction = mMemoryMapper.readByte(mProgramAddress);
    OpCode opCode = OP_CODE[instruction];
    // Fetch address of OpCode data
    Address opCodeDataAddress = getAddressOfOpCodeData(opCode);
    // Log the OpCode
    trace(opCode);

    switch (instruction) {
        case(0x00):  // BRK
        {
            if (mCpuStatus.emulationFlag()) {
                mStack.push16Bit(mProgramAddress.offset+2);
                mStack.push8Bit(mCpuStatus.getRegisterValue());
                setProgramAddress(0x00, mEmulationInterrupts->brkIrq);
                addToCycles(7);
            } else {
                mStack.push8Bit(mProgramAddress.bank);
                mStack.push16Bit(mProgramAddress.offset+2);
                mStack.push8Bit(mCpuStatus.getRegisterValue());
                setProgramAddress(0x00, mNativeInterrupts->brk);
                addToCycles(8);
            }
            // This is done by the 65C02 but not by the NMOS 6502
            mCpuStatus.clearDecimalFlag();
            mCpuStatus.setInterruptDisableFlag();
            break;
        }
        case(0x01):  // ORA DP Indexed Indirect, X
        {
            executeORA(opCode);
            break;
        }
        case (0x03): // ORA Stack Relative
        {
            executeORA(opCode);
            break;
        }
        case(0x05):  // ORA Direct Page
        {
            executeORA(opCode);
            break;
        }
        case(0x08):  // PHP
        {
            mStack.push8Bit(mCpuStatus.getRegisterValue());
            addToProgramAddressAndCycles(1,3);
            break;
        }
        case(0x07):  // ORA Direct page indirect long
        {
            executeORA(opCode);
            break;
        }
        case(0x09):  // ORA Immediate
        {
            executeORA(opCode);
            break;
        }
        case(0x0B):  // PHD
        {
            mStack.push16Bit(mD);
            addToProgramAddressAndCycles(1,4);
            break;
        }
        case(0x0D):  // ORA absolute
        {
            executeORA(opCode);
            break;
        }
        case(0x0F):  // ORA absolute long
        {
            executeORA(opCode);
            break;
        }
        case(0x10):  // BPL
        {
            int cycles = executeBranchShortOnCondition(!mCpuStatus.signFlag(), opCode);
            addToCycles(cycles);
            break;
        }
        case(0x11):  // ORA DP Indirect Indexed, Y
        {
            executeORA(opCode);
            break;
        }
        case(0x12):  // ORA Direct page indirect
        {
            executeORA(opCode);
            break;
        }
        case(0x13):  // SR Indirect Indexed, Y
        {
            executeORA(opCode);
            break;
        }
        case(0x15):  // ORA DP Indexed, X
        {
            executeORA(opCode);
            break;
        }
        case(0x17):  // ORA DP Indirect Long Indexed, Y
        {
            executeORA(opCode);
            break;
        }
        case(0x18):  // CLC
        {
            mCpuStatus.clearCarryFlag();
            addToProgramAddressAndCycles(1,2);
            break;
        }
        case(0x19):  // ORA Absolute Indexed, Y
        {
            executeORA(opCode);
            break;
        }
        case(0x1B):  // TCS
        {
            uint16_t newStackPointer;
            if (mCpuStatus.emulationFlag()) {
                newStackPointer = mStack.getStackPointer();
                newStackPointer &= 0xFF00;
                newStackPointer |= Binary::lower8BitsOf(mA);
            } else {
                newStackPointer = mA;
            }
            mStack = Stack(&mMemoryMapper, newStackPointer);
            addToProgramAddressAndCycles(1,2);
            break;
        }
        case(0x1D):  // ORA Absolute Indexed, X
        {
            executeORA(opCode);
            break;
        }
        case(0x1F):  // ORA Absolute Long Indexed, X
        {
            executeORA(opCode);
            break;
        }
        case(0x20):  // JSR addr
        {
            mStack.push16Bit(mProgramAddress.offset+2);
            Address destinationAddress = getAddressOfOpCodeData(opCode);
            setProgramAddress(destinationAddress);
            addToCycles(6);
            break;
        }
        case(0x22):  // JSR absolute long
        {
            Address destinationAddress = getAddressOfOpCodeData(opCode);
            mStack.push8Bit(mProgramAddress.bank);
            mStack.push16Bit(mProgramAddress.offset+3);
            setProgramAddress(destinationAddress);
            addToCycles(8);
            break;
        }
        case(0x28):  // PLP
        {
            mCpuStatus.setRegisterValue(mStack.pull8Bit());
            addToProgramAddressAndCycles(1,4);
            break;
        }
        case(0x29):  // AND #const
        {
            if (accumulatorIs8BitWide()) {
                mA = mMemoryMapper.readByte(opCodeDataAddress);
                mCpuStatus.updateSignAndZeroFlagFrom8BitValue(Binary::lower8BitsOf(mA));
                addToProgramAddressAndCycles(2,2);
            } else {
                mA = mMemoryMapper.readTwoBytes(opCodeDataAddress);
                mCpuStatus.updateSignAndZeroFlagFrom16BitValue(mA);
                addToProgramAddressAndCycles(3,3);
            }
            break;
        }
        case(0x2B):  // PLD
        {
            mD = mStack.pull16Bit();
            mCpuStatus.updateSignAndZeroFlagFrom16BitValue(mD);
            addToProgramAddressAndCycles(1,5);
            break;
        }
        case(0x30):  // BMI
        {
            addToCycles(executeBranchShortOnCondition(mCpuStatus.signFlag(), opCode));
            break;
        }
        case(0x48):  // PHA
        {
            if (accumulatorIs8BitWide()) {
                mStack.push8Bit(Binary::lower8BitsOf(mA));
            } else {
                mStack.push16Bit(mA);
            }
            addToProgramAddressAndCycles(1,3);
            break;
        }
        case(0x4C):  // JMP (absolute program)
        {
            setProgramAddress(opCodeDataAddress);
            addToCycles(3);
            break;
        }
        case(0x50):  // BVC
        {
            addToCycles(executeBranchShortOnCondition(!mCpuStatus.overflowFlag(), opCode));
            break;
        }
        case(0x5B):  // TCD
        {
            mD = mA;
            mCpuStatus.updateZeroFlagFrom16BitValue(mD);
            mCpuStatus.updateSignFlagFrom16BitValue(mD);
            addToProgramAddressAndCycles(1,2);
            break;
        }
        case(0x5C):  // JMP absolute long
        {
            setProgramAddress(opCodeDataAddress);
            addToCycles(4);
            break;
        }
        case(0x60):  // RTS
        {
            uint16_t offset = mStack.pull16Bit();
            setProgramAddress(mProgramAddress.bank, offset + 1);
            addToCycles(6);
            break;
        }
        case(0x62):  // PER
        {
            int opCodeSize = 3;
            mStack.push16Bit(mProgramAddress.offset + opCodeSize + mMemoryMapper.readTwoBytes(opCodeDataAddress));
            addToProgramAddressAndCycles(opCodeSize,6);
            break;
        }
        case(0x64):  // STZ direct page
        {
            int opCycles = Binary::lower8BitsOf(mD) != 0 ? 1 : 0;
            if (accumulatorIs8BitWide()) {
                mMemoryMapper.storeByte(opCodeDataAddress, (uint8_t)0x00);
                opCycles += 3;
            } else {
                mMemoryMapper.storeTwoBytes(opCodeDataAddress, (uint16_t)0x0000);
                opCycles += 4;
            }
            addToProgramAddressAndCycles(2, opCycles);
            break;
        }
        case(0x68):  // PLA
        {
            if (accumulatorIs8BitWide()) {
                mA = mStack.pull8Bit();
                mCpuStatus.updateSignAndZeroFlagFrom8BitValue(mA);
                addToProgramAddressAndCycles(1,4);
            } else {
                mA = mStack.pull16Bit();
                mCpuStatus.updateSignAndZeroFlagFrom16BitValue(mA);
                addToProgramAddressAndCycles(1,5);
            }
            break;
        }
        case(0x6B):  // RTL
        {
            Address returnAddress;
            returnAddress.offset = mStack.pull16Bit();
            returnAddress.bank = mStack.pull8Bit();
            setProgramAddress(returnAddress);
            addToProgramAddressAndCycles(1,6);
            break;
        }
        case(0x70):  // BVS
        {
            addToCycles(executeBranchShortOnCondition(mCpuStatus.overflowFlag(), opCode));
            break;
        }
        case(0x78):  // SEI
        {
            mCpuStatus.setInterruptDisableFlag();
            addToProgramAddressAndCycles(1,2);
            break;
        }
        case(0x80):  // BRA
        {
            addToCycles(executeBranchShortOnCondition(true, opCode));
            break;
        }
        case(0x82):  // BRL
        {
            addToCycles(executeBranchLongOnCondition(true, opCode));
            break;
        }
        case(0x85):  // STA direct page
        {
            int cycles = Binary::is8bitValueZero(Binary::lower8BitsOf(mD)) ? 3 : 4;
            if (accumulatorIs8BitWide()) {
                mMemoryMapper.storeByte(opCodeDataAddress, Binary::lower8BitsOf(mA));
                addToProgramAddressAndCycles(2, cycles);
            } else {
                mMemoryMapper.storeTwoBytes(opCodeDataAddress, mA);
                addToProgramAddressAndCycles(2, cycles+1);
            }
            break;
        }
        case(0x88):  // DEY
        {
            if (indexIs8BitWide()) {
                uint8_t lowerY = Binary::lower8BitsOf(mY);
                lowerY--;
                mY &= 0xFF00;
                mY |= lowerY;
                mCpuStatus.updateSignAndZeroFlagFrom8BitValue(lowerY);
            } else {
                mY--;
                mCpuStatus.updateSignAndZeroFlagFrom16BitValue(mY);
            }
            addToProgramAddressAndCycles(1,2);
            break;
        }
        case(0x8B):  // PHB
        {
            mStack.push8Bit(mDB);
            addToProgramAddressAndCycles(1,3);
            break;
        }
        case(0x8D):  // STA addr
        {
            if (accumulatorIs8BitWide()) {
                mMemoryMapper.storeByte(opCodeDataAddress, Binary::lower8BitsOf(mA));
                addToProgramAddressAndCycles(3,4);
            } else {
                mMemoryMapper.storeTwoBytes(opCodeDataAddress, mA);
                addToProgramAddressAndCycles(3,5);
            }
            break;
        }
        case(0x9C):  // STZ absolute
        {
            if (accumulatorIs8BitWide()) {
                mMemoryMapper.storeByte(opCodeDataAddress, (uint8_t)0x00);
                addToProgramAddressAndCycles(3, 4);
            } else {
                mMemoryMapper.storeTwoBytes(opCodeDataAddress, (uint16_t)0x0000);
                addToProgramAddressAndCycles(3, 5);
            }
            break;
        }
        case(0x9E):  // STZ absolute indexed, X
        {
            if (accumulatorIs8BitWide()) {
                mMemoryMapper.storeByte(opCodeDataAddress, (uint8_t)0x00);
                addToProgramAddressAndCycles(3, 5);
            } else {
                mMemoryMapper.storeTwoBytes(opCodeDataAddress, (uint16_t)0x0000);
                addToProgramAddressAndCycles(3, 6);
            }
            break;
        }
        case(0xA2):  // LDX #const
        {
            if (indexIs8BitWide()) {
                mX = mMemoryMapper.readByte(getAddressOfOpCodeData(opCode));
                mCpuStatus.updateSignAndZeroFlagFrom8BitValue(Binary::lower8BitsOf(mX));
                addToProgramAddressAndCycles(2,2);
            } else {
                mX = mMemoryMapper.readTwoBytes(getAddressOfOpCodeData(opCode));
                mCpuStatus.updateSignAndZeroFlagFrom16BitValue(mX);
                addToProgramAddressAndCycles(3,3);
            }
            break;
        }
        case(0xA9):  // LDA #const
        {
            if (accumulatorIs8BitWide()) {
                mA = mMemoryMapper.readByte(opCodeDataAddress);
                mCpuStatus.updateSignAndZeroFlagFrom8BitValue(Binary::lower8BitsOf(mA));
                addToProgramAddressAndCycles(2,2);
            } else {
                mA = mMemoryMapper.readTwoBytes(opCodeDataAddress);
                mCpuStatus.updateSignAndZeroFlagFrom16BitValue(mA);
                addToProgramAddressAndCycles(3,2);
            }
            break;
        }
        case(0xAB):  // PLB
        {
            mDB = mStack.pull8Bit();
            mCpuStatus.updateSignAndZeroFlagFrom8BitValue(mDB);
            addToProgramAddressAndCycles(1,4);
            break;
        }
        case(0xAF):  // LDA Absolute Long
        {
            if (accumulatorIs8BitWide()) {
                mA = mMemoryMapper.readByte(opCodeDataAddress);
                mCpuStatus.updateSignAndZeroFlagFrom8BitValue(Binary::lower8BitsOf(mA));
                addToProgramAddressAndCycles(4, 5);
            } else {
                mA = mMemoryMapper.readTwoBytes(opCodeDataAddress);
                mCpuStatus.updateSignAndZeroFlagFrom16BitValue(mA);
                addToProgramAddressAndCycles(4, 6);
            }
            break;
        }
        case(0xBF):  // LDA Absolute Long Indexed X
        {
            if (accumulatorIs8BitWide()) {
                mA = mMemoryMapper.readByte(opCodeDataAddress);
                mCpuStatus.updateSignAndZeroFlagFrom8BitValue(Binary::lower8BitsOf(mA));
                addToProgramAddressAndCycles(4, 5);
            } else {
                mA = mMemoryMapper.readTwoBytes(opCodeDataAddress);
                mCpuStatus.updateSignAndZeroFlagFrom16BitValue(mA);
                addToProgramAddressAndCycles(4, 6);
            }
            break;
        }
        case(0xC2):  // REP #const
        {
            uint8_t value = mMemoryMapper.readByte(getAddressOfOpCodeData(opCode));
            uint8_t statusByte = mCpuStatus.getRegisterValue();
            mCpuStatus.setRegisterValue(statusByte & ~value);
            addToProgramAddressAndCycles(2,3);
            break;
        }
        case(0xC8):  // INY
        {
            if (indexIs8BitWide()) {
                uint8_t lowerY = Binary::lower8BitsOf(mY);
                lowerY++;
                mY &= 0xFF00;
                mY |= lowerY;
                mCpuStatus.updateSignAndZeroFlagFrom8BitValue(lowerY);
            } else {
                mY++;
                mCpuStatus.updateSignAndZeroFlagFrom16BitValue(mY);
            }
            addToProgramAddressAndCycles(1,2);
            break;
        }
        case(0xCA):  // DEX
        {
            if (indexIs8BitWide()) {
                uint8_t lowerX = Binary::lower8BitsOf(mX);
                lowerX--;
                mX &= 0xFF00;
                mX |= lowerX;
                mCpuStatus.updateSignAndZeroFlagFrom8BitValue(lowerX);
            } else {
                mX--;
                mCpuStatus.updateSignAndZeroFlagFrom16BitValue(mX);
            }
            addToProgramAddressAndCycles(1,2);
            break;
        }
        case(0xD0):  // BNE
        {
            addToCycles(executeBranchShortOnCondition(!mCpuStatus.zeroFlag(), opCode));
            break;
        }
        case(0xD8):  // CLD
        {
            mCpuStatus.clearDecimalFlag();
            addToProgramAddressAndCycles(1,2);
            break;
        }
        case(0xE0):  // CPX #const
        {
            if (indexIs16BitWide()) {
                uint16_t operand = mMemoryMapper.readTwoBytes(opCodeDataAddress);
                uint16_t value = mX - operand;
                mCpuStatus.updateSignAndZeroFlagFrom16BitValue(value);
                if (mX < operand) mCpuStatus.clearCarryFlag();
                else mCpuStatus.setCarryFlag();
                addToProgramAddressAndCycles(3,3);

            } else {
                uint8_t operand = mMemoryMapper.readByte(opCodeDataAddress);
                uint8_t value = Binary::lower8BitsOf(mX) - operand;
                mCpuStatus.updateSignAndZeroFlagFrom8BitValue(value);
                if (Binary::lower8BitsOf(mX) < operand) mCpuStatus.clearCarryFlag();
                else mCpuStatus.setCarryFlag();
                addToProgramAddressAndCycles(2,2);
            }
            break;
        }
        case(0xE2):  // SEP #const
        {
            uint8_t value = mMemoryMapper.readByte(opCodeDataAddress);
            uint8_t statusByte = mCpuStatus.getRegisterValue();
            mCpuStatus.setRegisterValue(statusByte | value);
            addToProgramAddressAndCycles(2,3);
            break;
        }
        case(0xE8):  // INX
        {
            if (indexIs8BitWide()) {
                uint8_t lowerX = Binary::lower8BitsOf(mX);
                lowerX++;
                mX &= 0xFF00;
                mX |= lowerX;
                mCpuStatus.updateSignAndZeroFlagFrom8BitValue(lowerX);
            } else {
                mX++;
                mCpuStatus.updateSignAndZeroFlagFrom16BitValue(mX);
            }
            addToProgramAddressAndCycles(1,2);
            break;
        }
        case(0xF0):  // BEQ
        {

            addToCycles(executeBranchShortOnCondition(mCpuStatus.zeroFlag(), opCode));
            break;
        }
        case(0xFB):  // XCE
        {
            bool oldCarry = mCpuStatus.carryFlag();
            bool oldEmulation = mCpuStatus.emulationFlag();
            if (oldCarry) mCpuStatus.setEmulationFlag();
            else mCpuStatus.clearEmulationFlag();
            if (oldEmulation) mCpuStatus.setCarryFlag();
            else mCpuStatus.clearCarryFlag();

            mX &= 0xFF;
            mY &= 0xFF;

            if (mCpuStatus.emulationFlag()) {
                mCpuStatus.setAccumulatorWidthFlag();
                mCpuStatus.setIndexWidthFlag();
            } else {
                mCpuStatus.clearAccumulatorWidthFlag();
                mCpuStatus.clearIndexWidthFlag();
            }

            // New stack
            mStack = Stack(&mMemoryMapper);

            addToProgramAddressAndCycles(1,2);
            break;
        }

        /*
        case(0x9A):  // TXS
        {
            if (mEmulationFlag) {
                mStack = Stack(&mMemoryMapper, 0x100 | lower8BitsOf(mX));
            } else if (index8Bits()) {
                mStack = Stack(&mMemoryMapper, lower8BitsOf(mX));
            } else {
                mStack = Stack(&mMemoryMapper, mX);
            }
            addToProgramAddressAndCycles(1, 2);
            break;
        }/*
        case(0x9B):  // TXY
        {

            if (index8Bits()) {
                mY = lower8BitsOf(mX);
                updateSignFlagFromLower8BitsOf(mY);
                updateZeroFlagFromLower8BitsOf(mY);
            } else {
                mY = mX;
                updateSignFlagFrom16BitValue(mY);
                updateZeroFlagFrom16BitValue(mY);
            }
            addToProgramAddressAndCycles(1, 2);
            break;
        }
        case(0xBB):  // TYX
        {

            if (index8Bits()) {
                mY = lower8BitsOf(mX);
                updateSignFlagFromLower8BitsOf(mY);
                updateZeroFlagFromLower8BitsOf(mY);
            } else {
                mY = mX;
                updateSignFlagFrom16BitValue(mY);
                updateZeroFlagFrom16BitValue(mY);
            }
            addToProgramAddressAndCycles(1, 2);
            break;
        }
        case(0x8A):  // TXA
        {

            if ((index8Bits() && accumulator8Bits()) || (index16Bits() && accumulator8Bits())) {
                mA &= 0xFF00;
                mA |= lower8BitsOf(mX);
                updateSignFlagFromLower8BitsOf(lower8BitsOf(mA));
                updateZeroFlagFromLower8BitsOf(lower8BitsOf(mA));
            } else if (index8Bits() && accumulator16Bits()) {
                mA = lower8BitsOf(mX);
                updateSignFlagFromLower8BitsOf(lower8BitsOf(mA));
                updateZeroFlagFromLower8BitsOf(lower8BitsOf(mA));
            } else {
                mA = mX;
                updateSignFlagFrom16BitValue(mA);
                updateZeroFlagFrom16BitValue(mA);
            }
            addToProgramAddressAndCycles(1, 2);
            break;
        }
        case(0x98):  // TYA
        {

            if ((index8Bits() && accumulator8Bits()) || (index16Bits() && accumulator8Bits())) {
                mA &= 0xFF00;
                mA |= lower8BitsOf(mY);
                updateSignFlagFromLower8BitsOf(mA);
                updateZeroFlagFromLower8BitsOf(mA);
            } else if (index8Bits() && accumulator16Bits()) {
                mA = lower8BitsOf(mY);
                updateSignFlagFromLower8BitsOf(mA);
                updateZeroFlagFromLower8BitsOf(mA);
            } else {
                mA = mY;
                updateSignFlagFrom16BitValue(mA);
                updateZeroFlagFrom16BitValue(mA);
            }
            addToProgramAddressAndCycles(1, 2);
            break;
        }
        case(0x3B):  // TSA
        {

            mA = mStack.getStackPointer();
            updateSignFlagFrom16BitValue(mA);
            updateZeroFlagFrom16BitValue(mA);
            addToProgramAddressAndCycles(1, 2);
            break;
        }
        case(0xBA):  // TSX
        {

            if (index8Bits() || mEmulationFlag) {
                mX = lower8BitsOf(mStack.getStackPointer());
                updateSignFlagFromLower8BitsOf(lower8BitsOf(mX));
                updateZeroFlagFromLower8BitsOf(lower8BitsOf(mX));
            } else {
                mX = mStack.getStackPointer();
                updateSignFlagFrom16BitValue(mX);
                updateZeroFlagFrom16BitValue(mX);
            }
            addToProgramAddressAndCycles(1, 2);
            break;
        }*/
        /*
        case(0x8F):  // STA absolute long
        {
            uint16_t offset = mMemoryMapper.readTwoBytes(mPB, mPC+1);
            uint8_t bank = mMemoryMapper.readByte(mPB, mPC+3);

            if (accumulator8Bits()) {
                mMemoryMapper.storeByte(0x00, offset, lower8BitsOf(mA));
                addToProgramAddressAndCycles(4,5);
            } else {
                mMemoryMapper.storeTwoBytes(0x00, offset, mA);
                addToProgramAddressAndCycles(4,6);
            }

            break;
        }
        case(0x90):  // BCC
        {

            addToCycles(executeBranchOnCondition(!(mP&STATUS_CARRY), opCode));
            break;
        }
        case(0x99):  // STA absolute indexed, Y
        {
            uint16_t offset = mMemoryMapper.readTwoBytes(mPB, mPC+1);

            if (accumulator8Bits()) {
                mMemoryMapper.storeByte(mDB, offset + indexWithYRegister(), lower8BitsOf(mA));
                addToProgramAddressAndCycles(3, 5);
            } else {
                mMemoryMapper.storeTwoBytes(mDB, offset + indexWithYRegister(), mA);
                addToProgramAddressAndCycles(3, 6);
            }
            break;
        }*/


        /*
        case(0x9F):  // STA absolute long indexed, X
        {
            uint16_t offset = mMemoryMapper.readTwoBytes(mPB, mPC+1);
            uint8_t bank = mMemoryMapper.readByte(mPB, mPC+3);

            if (accumulator8Bits()) {
                mMemoryMapper.storeByte(bank, offset + indexWithXRegister(), lower8BitsOf(mA));
                addToProgramAddressAndCycles(4, 5);
            } else {
                mMemoryMapper.storeTwoBytes(bank, offset + indexWithXRegister(), mA);
                addToProgramAddressAndCycles(4, 6);
            }
            break;
        }
        case(0xA0):  // LDY #const
        {
            if (index8Bits()) {
                mY = mMemoryMapper.readByte(mPB, mPC+1);

                updateZeroFlagFromLower8BitsOf(mY);
                updateSignFlagFromLower8BitsOf(mY);
                addToProgramAddressAndCycles(2,2);
            } else {
                mY = mMemoryMapper.readTwoBytes(mPB, mPC+1);

                updateZeroFlagFrom16BitValue(mY);
                updateSignFlagFrom16BitValue(mY);
                addToProgramAddressAndCycles(3,3);
            }
            break;
        }*/
        /*
        case(0xA8):  // TAY
        {

            if ((accumulator8Bits() && index8Bits()) || (accumulator16Bits() && index8Bits())) {
                mY = lower8BitsOf(mA);
                updateZeroFlagFromLower8BitsOf(mY);
                updateSignFlagFromLower8BitsOf(mY);
            } else {
                mY = mA;
                updateZeroFlagFrom16BitValue(mY);
                updateSignFlagFrom16BitValue(mY);
            }
            addToProgramAddressAndCycles(1,2);
            break;
        }
        case(0xAA):  // TAX
        {

            if ((accumulator8Bits() && index8Bits()) || (accumulator16Bits() && index8Bits())) {
                mX = lower8BitsOf(mA);
                updateZeroFlagFromLower8BitsOf(mX);
                updateSignFlagFromLower8BitsOf(mX);
            } else {
                mX = mA;
                updateZeroFlagFrom16BitValue(mX);
                updateSignFlagFrom16BitValue(mX);
            }
            addToProgramAddressAndCycles(1,2);
            break;
        }*/

        /*
        case(0xB0):  // BCS
        {

            addToCycles(executeBranchOnCondition((mP&STATUS_CARRY), opCode));
            break;
        }*/
        /*
        case(0xEC):  // CPX #addr
        {
            uint16_t offset = mMemoryMapper.readTwoBytes(mPB, mPC+1);

            if (index16Bits()) {
                uint16_t operand = mMemoryMapper.readTwoBytes(mDB, offset);
                uint16_t value = mX - operand;
                updateZeroFlagFrom16BitValue(value);
                updateSignFlagFrom16BitValue(value);
                if (mX < operand) clearCarryFlag();
                else setCarryFlag();
                addToProgramAddressAndCycles(3,5);
            } else {
                uint8_t operand = mMemoryMapper.readByte(mDB, offset);
                uint8_t value = lower8BitsOf(mX) - operand;
                updateZeroFlagFromLower8BitsOf(value);
                updateSignFlagFromLower8BitsOf(value);
                if (lower8BitsOf(mX) < operand) clearCarryFlag();
                else setCarryFlag();
                addToProgramAddressAndCycles(3,4);
            }
            break;
        }
        case(0xE4):  // CPX direct page
        {
            uint8_t offset = mMemoryMapper.readByte(mPB, mPC+1);

            int opCycles = mD != 0 ? 1 : 0;
            if (index16Bits()) {
                uint16_t operand = mMemoryMapper.readTwoBytes(mDB, mD + offset);
                uint16_t value = mX - operand;
                updateZeroFlagFrom16BitValue(value);
                updateSignFlagFrom16BitValue(value);
                if (mX < value) clearCarryFlag();
                else setCarryFlag();
                addToProgramAddressAndCycles(2,4 + opCycles);
            } else {
                uint8_t operand = mMemoryMapper.readByte(mDB, mD + offset);
                uint8_t value = lower8BitsOf(mX) - operand;
                updateZeroFlagFromLower8BitsOf(value);
                updateSignFlagFromLower8BitsOf(value);
                if (lower8BitsOf(mX) < value) clearCarryFlag();
                else setCarryFlag();
                addToProgramAddressAndCycles(2,3 + opCycles);
            }
            break;
        }
        case(0xC0):  // CPY #const
        {
            if (index16Bits()) {
                uint16_t operand = mMemoryMapper.readTwoBytes(mPB, mPC+1);

                uint16_t value = mY - operand;
                updateZeroFlagFrom16BitValue(value);
                updateSignFlagFrom16BitValue(value);
                if (mY < operand) clearCarryFlag();
                else setCarryFlag();
                addToProgramAddressAndCycles(3,3);

            } else {
                uint8_t operand = mMemoryMapper.readByte(mPB, mPC+1);

                uint8_t value = lower8BitsOf(mY) - operand;
                updateZeroFlagFromLower8BitsOf(value);
                updateSignFlagFromLower8BitsOf(value);
                if (lower8BitsOf(mY) < operand) clearCarryFlag();
                else setCarryFlag();
                addToProgramAddressAndCycles(2,2);
            }
            break;
        }
        case(0xCC):  // CPY #addr
        {
            uint16_t offset = mMemoryMapper.readTwoBytes(mPB, mPC+1);

            if (index16Bits()) {
                uint16_t operand = mMemoryMapper.readTwoBytes(mDB, offset);
                uint16_t value = mY - operand;
                updateZeroFlagFrom16BitValue(value);
                updateSignFlagFrom16BitValue(value);
                if (mY < operand) clearCarryFlag();
                else setCarryFlag();
                addToProgramAddressAndCycles(3,5);
            } else {
                uint8_t operand = mMemoryMapper.readByte(mDB, offset);
                uint8_t value = lower8BitsOf(mY) - operand;
                updateZeroFlagFromLower8BitsOf(value);
                updateSignFlagFromLower8BitsOf(value);
                if (lower8BitsOf(mY) < operand) clearCarryFlag();
                else setCarryFlag();
                addToProgramAddressAndCycles(3,4);
            }
            break;
        }
        case(0xC4):  // CPY direct page
        {
            uint8_t offset = mMemoryMapper.readByte(mPB, mPC+1);

            int opCycles = mD != 0 ? 1 : 0;
            if (index16Bits()) {
                uint16_t operand = mMemoryMapper.readTwoBytes(mDB, mD + offset);
                uint16_t value = mY - operand;
                updateZeroFlagFrom16BitValue(value);
                updateSignFlagFrom16BitValue(value);
                if (mY < value) clearCarryFlag();
                else setCarryFlag();
                addToProgramAddressAndCycles(2,4 + opCycles);
            } else {
                uint8_t operand = mMemoryMapper.readByte(mDB, mD + offset);
                uint8_t value = lower8BitsOf(mY) - operand;
                updateZeroFlagFromLower8BitsOf(value);
                updateSignFlagFromLower8BitsOf(value);
                if (lower8BitsOf(mY) < value) clearCarryFlag();
                else setCarryFlag();
                addToProgramAddressAndCycles(2,3 + opCycles);
            }
            break;
        }
        case(0x8C):  // STY #addr
        {
            uint16_t offset = mMemoryMapper.readTwoBytes(mPB, mPC+1);

            if (index16Bits()) {
                mMemoryMapper.storeTwoBytes(mDB, offset, mY);
                addToProgramAddressAndCycles(3,5);
            } else {
                mMemoryMapper.storeByte(mDB, offset, lower8BitsOf(mY));
                addToProgramAddressAndCycles(3,4);
            }
            break;
        }

        case(0x84):  // STY direct page
        {
            int opCycles = mD != 0 ? 1 : 0; // CHECK should be lower8BitsOf?
            uint8_t offset = mMemoryMapper.readByte(mPB, mPC+1);

            if (index16Bits()) {
                mMemoryMapper.storeTwoBytes(mDB, mD + offset, mY);
                addToProgramAddressAndCycles(2, opCycles + 4);
            } else {
                mMemoryMapper.storeByte(mDB, mD + offset, lower8BitsOf(mY));
                addToProgramAddressAndCycles(2, opCycles + 3);
            }
            break;
        }
        case(0x94):  // STY direct page indexed, X
        {
            int opCycles = mD != 0 ? 1 : 0; // CHECK should be lower8BitsOf?
            uint8_t offset = mMemoryMapper.readByte(mPB, mPC+1);

            if (index16Bits()) {
                mMemoryMapper.storeTwoBytes(mDB, mD + indexWithXRegister() + offset, mY);
                addToProgramAddressAndCycles(2, opCycles + 5);
            } else {
                mMemoryMapper.storeByte(mDB, mD + indexWithXRegister() + offset, lower8BitsOf(mY));
                addToProgramAddressAndCycles(2, opCycles + 4);
            }
            break;
        }
        case(0x8E):  // STX #addr
        {
            uint16_t offset = mMemoryMapper.readTwoBytes(mPB, mPC+1);

            if (index16Bits()) {
                mMemoryMapper.storeTwoBytes(mDB, offset, mX);
                addToProgramAddressAndCycles(3,5);
            } else {
                mMemoryMapper.storeByte(mDB, offset, lower8BitsOf(mX));
                addToProgramAddressAndCycles(3,4);
            }
            break;
        }

        case(0x86):  // STX direct page
        {
            int opCycles = mD != 0 ? 1 : 0;
            uint8_t offset = mMemoryMapper.readByte(mPB, mPC+1);

            if (index16Bits()) {
                mMemoryMapper.storeTwoBytes(mDB, mD + offset, mX);
                addToProgramAddressAndCycles(2, opCycles + 4);
            } else {
                mMemoryMapper.storeByte(mDB, mD + offset, lower8BitsOf(mX));
                addToProgramAddressAndCycles(2, opCycles + 3);
            }
            break;
        }
        case(0x96):  // STX direct page indexed, X
        {
            int opCycles = mD != 0 ? 1 : 0;
            uint8_t offset = mMemoryMapper.readByte(mPB, mPC+1);

            if (index16Bits()) {
                mMemoryMapper.storeTwoBytes(mDB, mD + indexWithYRegister() + offset, mX);
                addToProgramAddressAndCycles(2, opCycles + 5);
            } else {
                mMemoryMapper.storeByte(mDB, mD + indexWithYRegister() + offset, lower8BitsOf(mX));
                addToProgramAddressAndCycles(2, opCycles + 4);
            }
            break;
        }*/




        /*
        case(0xEB):  // XBA
        {
            uint8_t lowerA = lower8BitsOf(mA);
            uint8_t higherA = higher8BitsOf(mA);
            mA = higherA | ((uint16_t)(lowerA) << 8);
            updateSignFlagFromLower8BitsOf(mA);
            updateZeroFlagFromLower8BitsOf(mA);
            addToProgramAddressAndCycles(1,3);
            return false;
            break;
        }

#define doCMPWith8BitValue(__value) { \
    uint8_t result = lower8BitsOf(mA) - __value; \
    updateSignFlagFromLower8BitsOf(result); \
    updateZeroFlagFromLower8BitsOf(result); \
    if (lower8BitsOf(mA) >= __value) {clearCarryFlag();} \
    else {setCarryFlag();} \
}

#define doCMPWith16BitValue(__value) { \
    uint16_t result = mA - __value; \
    updateSignFlagFrom16BitValue(result); \
    updateZeroFlagFrom16BitValue(result); \
    if (mA >= __value) clearCarryFlag(); \
    else setCarryFlag(); \
}
        case(0xC9):  // CMP #const
        {
            if (accumulator8Bits()) {
                uint8_t operand = mMemoryMapper.readByte(mPB, mPC+1);

                doCMPWith8BitValue(operand);
                addToProgramAddressAndCycles(2,2);
            } else {
                uint16_t operand = mMemoryMapper.readTwoBytes(mPB, mPC+1);

                doCMPWith16BitValue(operand);
                addToProgramAddressAndCycles(3,3);
            }
            break;
        }
        case(0xCD):  // CMP addr
        {
            uint16_t offset = mMemoryMapper.readTwoBytes(mPB, mPC+1);

            if (accumulator8Bits()) {
                uint8_t operand = mMemoryMapper.readByte(mDB, offset);
                doCMPWith8BitValue(operand);
                addToProgramAddressAndCycles(3,4);
            } else {
                uint16_t operand = mMemoryMapper.readTwoBytes(mDB, offset);
                doCMPWith16BitValue(operand);
                addToProgramAddressAndCycles(3,5);
            }
            break;
        }
        case(0xCF):  // CMP long
        {
            uint16_t offset = mMemoryMapper.readTwoBytes(mPB, mPC+1);
            uint8_t bank = mMemoryMapper.readByte(mPB, mPC+3);

            if (accumulator8Bits()) {
                uint8_t operand = mMemoryMapper.readByte(bank, offset);
                doCMPWith8BitValue(operand);
                addToProgramAddressAndCycles(4,5);
            } else {
                uint16_t operand = mMemoryMapper.readTwoBytes(bank, offset);
                doCMPWith16BitValue(operand);
                addToProgramAddressAndCycles(4,6);
            }
            break;
        }
        case(0xC5):  // CMP direct page
        {
            uint8_t offset = mMemoryMapper.readByte(mPB, mPC+1);

            int opCycles = lower8BitsOf(mD) != 0 ? 1 : 0;
            if (accumulator8Bits()) {
                uint8_t operand = mMemoryMapper.readByte(mDB, mD + offset);
                doCMPWith8BitValue(operand);
                addToProgramAddressAndCycles(2,opCycles + 3);
            } else {
                uint16_t operand = mMemoryMapper.readTwoBytes(mDB, mD + offset);
                doCMPWith16BitValue(operand);
                addToProgramAddressAndCycles(2,opCycles + 4);
            }
            break;
        }
        case(0xD2):  // CMP direct page indirect
        {
            uint8_t dpOffset = mMemoryMapper.readByte(mPB, mPC+1);

            int opCycles = lower8BitsOf(mD) != 0 ? 1 : 0;
            if (accumulator8Bits()) {
                uint8_t operand;
                fetchDirectPageIndirect8BitOperand(dpOffset, operand);
                doCMPWith8BitValue(operand);
                addToProgramAddressAndCycles(2,opCycles+5);
            } else {
                uint16_t operand;
                fetchDirectPageIndirect16BitOperand(dpOffset, operand);
                doCMPWith16BitValue(operand);
                addToProgramAddressAndCycles(2,opCycles+6);
            }
            break;
        }
        case(0xC7):  // CMP direct page indirect long
        {
            uint8_t dpOffset = mMemoryMapper.readByte(mPB, mPC+1);

            int opCycles = lower8BitsOf(mD) != 0 ? 1 : 0;
            if (accumulator8Bits()) {
                uint8_t operand;
                fetchDirectPageIndirectLong8BitOperand(dpOffset, operand);
                doCMPWith8BitValue(operand);
                addToProgramAddressAndCycles(2,opCycles+6);
            } else {
                uint16_t operand;
                fetchDirectPageIndirectLong16BitOperand(dpOffset, operand);
                doCMPWith16BitValue(operand);
                addToProgramAddressAndCycles(2,opCycles+7);
            }
            break;
        }
        case(0xDD):  // CMP absolute indexed, X
        {
            uint16_t offset = mMemoryMapper.readTwoBytes(mPB, mPC+1);

            int opCycles = indexingWithXCrossesPageBoundary(offset) ? 1 : 0;
            if (accumulator8Bits()) {
                uint8_t operand = mMemoryMapper.readByte(mDB, offset + indexWithXRegister());
                doCMPWith8BitValue(operand);
                addToProgramAddressAndCycles(3,opCycles + 4);
            } else {
                uint16_t operand = mMemoryMapper.readTwoBytes(mDB, offset + indexWithXRegister());
                doCMPWith16BitValue(operand);
                addToProgramAddressAndCycles(3,opCycles + 5);
            }
            break;
        }
        case(0xDF):  // CMP absolute long indexed, X
        {
            uint16_t offset = mMemoryMapper.readTwoBytes(mPB, mPC+1);
            uint8_t bank = mMemoryMapper.readByte(mPB, mPC+3);

            if (accumulator8Bits()) {
                uint8_t operand = mMemoryMapper.readByte(bank, offset + indexWithXRegister());
                doCMPWith8BitValue(operand);
                addToProgramAddressAndCycles(4,5);
            } else {
                uint16_t operand = mMemoryMapper.readTwoBytes(bank, offset + indexWithXRegister());
                doCMPWith16BitValue(operand);
                addToProgramAddressAndCycles(4,6);
            }
            break;
        }
        case(0xD9):  // CMP absolute indexed, Y
        {
            uint16_t offset = mMemoryMapper.readTwoBytes(mPB, mPC+1);

            int opCycles = indexingWithYCrossesPageBoundary(offset) ? 1 : 0;
            if (accumulator8Bits()) {
                uint8_t operand = mMemoryMapper.readByte(mDB, offset + indexWithYRegister());
                doCMPWith8BitValue(operand);
                addToProgramAddressAndCycles(3,opCycles + 4);
            } else {
                uint16_t operand = mMemoryMapper.readTwoBytes(mDB, offset + indexWithYRegister());
                doCMPWith16BitValue(operand);
                addToProgramAddressAndCycles(3,opCycles + 5);
            }
            break;
        }
        case(0xD5):  // CMP direct page indexed, X
        {
            uint8_t offset = mMemoryMapper.readByte(mPB, mPC+1);

            int opCycles = lower8BitsOf(mD) != 0 ? 1 : 0;
            if (accumulator8Bits()) {
                uint8_t operand = mMemoryMapper.readByte(mDB, mD + indexWithXRegister() + offset);
                doCMPWith8BitValue(operand);
                addToProgramAddressAndCycles(2,4+opCycles);
            } else {
                uint16_t operand = mMemoryMapper.readTwoBytes(mDB, mD + indexWithXRegister() + offset);
                doCMPWith16BitValue(operand);
                addToProgramAddressAndCycles(2,5+opCycles);
            }
            break;
        }
        case(0xC1):  // CMP direct page indexed indirect, X (preindexing)
        {
            uint8_t dpOffset = mMemoryMapper.readByte(mPB, mPC+1);

            int opCycles = lower8BitsOf(mD) != 0 ? 1 : 0;
            if (accumulator8Bits()) {
                uint8_t operand;
                fetchDirectPageIndirect8BitOperand(dpOffset + indexWithXRegister(), operand);
                doCMPWith8BitValue(operand);
                addToProgramAddressAndCycles(2,6+opCycles);
            } else {
                uint16_t operand;
                fetchDirectPageIndirect16BitOperand(dpOffset + indexWithXRegister(), operand);
                doCMPWith16BitValue(operand);
                addToProgramAddressAndCycles(2,7+opCycles);
            }
            break;
        }
        case(0xD1):  // CMP direct page indexed indirect, Y (postindexing)
        {
            // CHECK use PB for retrieving address from direct page? Or use bank zero always?
            uint8_t dpOffset = mMemoryMapper.readByte(mPB, mPC+1);

            uint16_t indirectAddressOffset = mMemoryMapper.readTwoBytes(mDB, mD + dpOffset);
            uint16_t indirectAddressOffsetPostIndexed = indirectAddressOffset + indexWithYRegister();
            int opCycles = lower8BitsOf(mD) != 0 ? 1 : 0;
            opCycles += offsetsAreOnDifferentPages(indirectAddressOffset, indirectAddressOffsetPostIndexed) ? 1 : 0;
            if (accumulator8Bits()) {
                uint8_t operand = mMemoryMapper.readByte(mDB, indirectAddressOffsetPostIndexed);
                doCMPWith8BitValue(operand);
                addToProgramAddressAndCycles(2,5+opCycles);
            } else {
                uint16_t operand = mMemoryMapper.readTwoBytes(mDB, indirectAddressOffsetPostIndexed);
                doCMPWith16BitValue(operand);
                addToProgramAddressAndCycles(2,6+opCycles);
            }
            break;
        }
        case(0xD7):  // CMP direct page indirect long indexed, Y
        {
            uint8_t dpOffset = mMemoryMapper.readByte(mPB, mPC+1);

            uint16_t indirectAddressOffset = mMemoryMapper.readTwoBytes(mDB, mD + dpOffset);
            uint8_t indirectAddressBank = mMemoryMapper.readByte(mDB, mD + dpOffset + 2);
            uint32_t fullIndirectAddress = indirectAddressOffset | ((uint32_t)indirectAddressBank << 16);
            uint32_t fullIndirectAddressPostIndexed = fullIndirectAddress + indexWithYRegister();
            uint16_t postIndexedOffset = fullIndirectAddressPostIndexed & 0x0000FFFF;
            uint8_t postIndexedBank = (fullIndirectAddressPostIndexed & 0x00FF0000) >> 16;
            int opCycles = lower8BitsOf(mD) != 0 ? 1 : 0;
            if (accumulator8Bits()) {
                uint8_t operand = mMemoryMapper.readByte(postIndexedBank, postIndexedOffset);
                doCMPWith8BitValue(operand);
                addToProgramAddressAndCycles(2,6+opCycles);
            } else {
                uint16_t operand = mMemoryMapper.readTwoBytes(postIndexedBank, postIndexedOffset);
                doCMPWith16BitValue(operand);
                addToProgramAddressAndCycles(2,7+opCycles);
            }
            break;
        }
        case(0xC3):  // CMP stack relative
        {
            uint8_t srOffset = mMemoryMapper.readByte(mPB, mPC+1);
            if (accumulator8Bits()) {
                uint8_t operand = mMemoryMapper.readByte(0x00, mStack.getStackPointer() + srOffset);
                doCMPWith8BitValue(operand);
                addToProgramAddressAndCycles(2,4);
            } else {
                uint16_t operand = mMemoryMapper.readTwoBytes(0x00, mStack.getStackPointer() + srOffset);
                doCMPWith16BitValue(operand);
                addToProgramAddressAndCycles(2,5);
            }
            break;
        }
        case(0xD3):  // CMP stack relative indirect indexed, Y
        {
            uint8_t srOffset = mMemoryMapper.readByte(mPB, mPC+1);

            uint16_t offset = mMemoryMapper.readTwoBytes(0x00, mStack.getStackPointer() + srOffset);
            offset += indexWithYRegister();
            if (accumulator8Bits()) {
                uint8_t operand = mMemoryMapper.readByte(mDB, offset);
                doCMPWith8BitValue(operand);
                addToProgramAddressAndCycles(2,7);
            } else {
                uint16_t operand = mMemoryMapper.readTwoBytes(mDB, offset);
                doCMPWith16BitValue(operand);
                addToProgramAddressAndCycles(2,8);
            }
            break;
        }
        * */

        default:
            Log::trc(LOG_TAG).str("Unimplemented!").show();
            return false;
            break;
    }

    // TODO: make decent debugger
    if (mBreakpointEnabled) {
        if (mProgramAddress.bank == mBreakBank && mProgramAddress.offset == mBreakOffset) return false;
    }

    return true;
}


#include "Cpu.hpp"

/**
 * This file contains the implementation for every stack related OpCode.
 */
void Cpu::executeStack(OpCode &opCode) {
    Address opCodeDataAddress = getAddressOfOpCodeData(opCode);
    switch (opCode.getCode()) {
        case 0xF4:                  // PEA
        {
            uint16_t operand = mMemoryMapper.readTwoBytes(opCodeDataAddress);
            mStack.push16Bit(operand);
            addToProgramAddressAndCycles(3, 5);
            break;
        }
        case 0xD4:                  // PEI
        {
            uint16_t operand = mMemoryMapper.readTwoBytes(opCodeDataAddress);
            mStack.push16Bit(operand);
            int opCycles = Binary::lower8BitsOf(mD) != 0 ? 1 : 0;
            addToProgramAddressAndCycles(2, 6+opCycles);
            break;
        }
        case 0x62:                  // PER
        {
            int opCodeSize = 3;
            uint16_t operand = mMemoryMapper.readTwoBytes(opCodeDataAddress);
            uint16_t sum = operand + opCodeSize + mProgramAddress.offset;
            mStack.push16Bit(sum);
            addToProgramAddressAndCycles(3, 6);
            break;
        }
        case(0x48):                 // PHA
        {
            if (accumulatorIs8BitWide()) {
                mStack.push8Bit(Binary::lower8BitsOf(mA));
                addToProgramAddressAndCycles(1, 4);
            } else {
                mStack.push16Bit(mA);
                addToProgramAddressAndCycles(1, 3);
            }
            break;
        }
        case(0x8B):                 // PHB
        {
            mStack.push8Bit(mDB);
            addToProgramAddressAndCycles(1, 3);
            break;
        }
        case(0x0B):                 // PHD
        {
            mStack.push16Bit(mD);
            addToProgramAddressAndCycles(1, 4);
            break;
        }
        case(0x4B):                 // PHK
        {
            mStack.push8Bit(mProgramAddress.bank);
            addToProgramAddressAndCycles(1, 3);
            break;
        }
        case(0x08):                 // PHP
        {
            mStack.push8Bit(mCpuStatus.getRegisterValue());
            addToProgramAddressAndCycles(1, 3);
            break;
        }
        case(0xDA):                 // PHX
        {
            if (indexIs8BitWide()) {
                mStack.push8Bit(Binary::lower8BitsOf(mX));
                addToProgramAddressAndCycles(1, 3);
            } else {
                mStack.push16Bit(mX);
                addToProgramAddressAndCycles(1, 4);
            }
            break;
        }
        case(0x5A):                 // PHY
        {
            if (indexIs8BitWide()) {
                mStack.push8Bit(Binary::lower8BitsOf(mY));
                addToProgramAddressAndCycles(1, 3);
            } else {
                mStack.push16Bit(mY);
                addToProgramAddressAndCycles(1, 4);
            }
            break;
        }
        case(0x68):                 // PLA
        {
            if (accumulatorIs8BitWide()) {
                Binary::setLower8BitsOf16BitsValue(&mA, mStack.pull8Bit());
                mCpuStatus.updateSignAndZeroFlagFrom8BitValue(mA);
                addToProgramAddressAndCycles(1, 4);
            } else {
                mA = mStack.pull16Bit();
                mCpuStatus.updateSignAndZeroFlagFrom16BitValue(mA);
                addToProgramAddressAndCycles(1, 5);
            }
            break;
        }
        case(0xAB):                 // PLB
        {
            mDB = mStack.pull8Bit();
            mCpuStatus.updateSignAndZeroFlagFrom8BitValue(mDB);
            addToProgramAddressAndCycles(1, 4);
            break;
        }
        case(0x2B):                 // PLD
        {
            mD = mStack.pull16Bit();
            mCpuStatus.updateSignAndZeroFlagFrom16BitValue(mD);
            addToProgramAddressAndCycles(1, 5);
            break;
        }
        case(0x28):                 // PLP
        {
            mCpuStatus.setRegisterValue(mStack.pull8Bit());
            addToProgramAddressAndCycles(1, 4);
            break;
        }
        case(0xFA):                 // PLX
        {
            if (indexIs8BitWide()) {
                uint8_t value = mStack.pull8Bit();
                Binary::setLower8BitsOf16BitsValue(&mX, value);
                mCpuStatus.updateSignAndZeroFlagFrom8BitValue(value);
                addToProgramAddressAndCycles(1, 4);
            } else {
                mX = mStack.pull16Bit();
                mCpuStatus.updateSignAndZeroFlagFrom16BitValue(mX);
                addToProgramAddressAndCycles(1, 5);
            }
            break;
        }
        case(0x7A):                 // PLY
        {
            if (indexIs8BitWide()) {
                uint8_t value = mStack.pull8Bit();
                Binary::setLower8BitsOf16BitsValue(&mY, value);
                mCpuStatus.updateSignAndZeroFlagFrom8BitValue(value);
                addToProgramAddressAndCycles(1, 4);
            } else {
                mY = mStack.pull16Bit();
                mCpuStatus.updateSignAndZeroFlagFrom16BitValue(mY);
                addToProgramAddressAndCycles(1, 5);
            }
            break;
        }
    }
}
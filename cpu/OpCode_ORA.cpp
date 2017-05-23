#include "Cpu.hpp"

#include <cmath>

/**
 * This file contains a bunch of methods used by ORA OpCodes.
 * Actual execution of the opcodes is performed inside Executor.cpp
 * These methods are called from there.
 */
void Cpu::executeORA8Bit(OpCode &opCode) {
    Address opCodeDataAddress = getAddressOfOpCodeData(opCode);
    uint8_t operand = mMemoryMapper.readByte(opCodeDataAddress);
    uint8_t result = Binary::lower8BitsOf(mA) | operand;
    mCpuStatus.updateSignAndZeroFlagFrom8BitValue(result);
    Binary::setLower8BitsOf16BitsValue(&mA, result);
}

void Cpu::executeORA16Bit(OpCode &opCode) {
    Address opCodeDataAddress = getAddressOfOpCodeData(opCode);
    uint16_t operand = mMemoryMapper.readTwoBytes(opCodeDataAddress);
    uint16_t result = mA | operand;
    mCpuStatus.updateSignAndZeroFlagFrom16BitValue(result);
    mA = result;
}

void Cpu::executeORA(OpCode &opCode) {
    switch (opCode.getCode()) {
        case (0x01):                // ORA DP Indexed Indirect, X
        {
            int opCycles = Binary::lower8BitsOf(mD) != 0 ? 1 : 0;
            if (accumulatorIs8BitWide()) {
                executeORA8Bit(opCode);
            } else {
                executeORA16Bit(opCode);
                opCycles++;
            }
            addToProgramAddressAndCycles(2, 6 + opCycles);
            break;
        }
        case (0x03):                // ORA Stack Relative
        {
            if (accumulatorIs8BitWide()) {
                executeORA8Bit(opCode);
                addToProgramAddressAndCycles(2, 4);
            } else {
                executeORA16Bit(opCode);
                addToProgramAddressAndCycles(2, 5);
            }
            break;
        }
        case(0x05):                 // ORA Direct Page
        {
            int opCycles = Binary::lower8BitsOf(mD) != 0 ? 1 : 0;
            if (accumulatorIs8BitWide()) {
                executeORA8Bit(opCode);
            } else {
                executeORA16Bit(opCode);
                opCycles++;
            }
            addToProgramAddressAndCycles(2, 3+opCycles);
            break;
        }
        case(0x07):                 // ORA Direct page indirect long
        {
            int opCycles = Binary::lower8BitsOf(mD) != 0 ? 1 : 0;
            if (accumulatorIs8BitWide()) {
                executeORA8Bit(opCode);
            } else {
                executeORA16Bit(opCode);
                opCycles++;
            }
            addToProgramAddressAndCycles(2, 6+opCycles);
            break;
        }
        case (0x09):                // ORA Immediate
        {
            if (accumulatorIs8BitWide()) {
                executeORA8Bit(opCode);
                addToProgramAddressAndCycles(2,2);
            } else {
                executeORA16Bit(opCode);
                addToProgramAddressAndCycles(3,3);
            }
            break;
        }
        case (0x0D):                // ORA absolute
        {
            if (accumulatorIs8BitWide()) {
                executeORA8Bit(opCode);
                addToProgramAddressAndCycles(3,4);
            } else {
                executeORA16Bit(opCode);
                addToProgramAddressAndCycles(3,5);
            }
            break;
        }
        case (0x0F):                // ORA absolute long
        {
            if (accumulatorIs8BitWide()) {
                executeORA8Bit(opCode);
                addToProgramAddressAndCycles(4,5);
            } else {
                executeORA16Bit(opCode);
                addToProgramAddressAndCycles(4,6);
            }
            break;
        }
        case(0x11):                 // ORA DP Indirect Indexed, Y
        {
            int opCycles = Binary::lower8BitsOf(mD) != 0 ? 1 : 0;
            opCycles += opCodeAddressingCrossesPageBoundary(opCode) ? 1 : 0;
            if (accumulatorIs8BitWide()) {
                executeORA8Bit(opCode);
            } else {
                executeORA16Bit(opCode);
                opCycles++;
            }
            addToProgramAddressAndCycles(2,5+opCycles);
            break;
        }
        case(0x12):                 // ORA Direct page indirect
        {
            int opCycles = Binary::lower8BitsOf(mD) != 0 ? 1 : 0;
            if (accumulatorIs8BitWide()) {
                executeORA8Bit(opCode);
            } else {
                executeORA16Bit(opCode);
                opCycles++;
            }
            addToProgramAddressAndCycles(2, 5+opCycles);
            break;
        }
        case(0x13):                 // SR Indirect Indexed, Y
        {
            int opCycles = Binary::lower8BitsOf(mD) != 0 ? 1 : 0;
            if (accumulatorIs8BitWide()) {
                executeORA8Bit(opCode);
                addToProgramAddressAndCycles(2,7);
            } else {
                executeORA16Bit(opCode);
                addToProgramAddressAndCycles(2,8);
            }
            break;
        }
        case(0x15):                 // ORA DP Indexed, X
        {
            int opCycles = Binary::lower8BitsOf(mD) != 0 ? 1 : 0;
            if (accumulatorIs8BitWide()) {
                executeORA8Bit(opCode);
                addToProgramAddressAndCycles(2,4+opCycles);
            } else {
                executeORA16Bit(opCode);
                addToProgramAddressAndCycles(2,5+opCycles);
            }
            break;
        }
        case(0x17):                 // ORA DP Indirect Long Indexed, Y
        {
            int opCycles = Binary::lower8BitsOf(mD) != 0 ? 1 : 0;
            if (accumulatorIs8BitWide()) {
                executeORA8Bit(opCode);
            } else {
                executeORA16Bit(opCode);
                opCycles++;
            }
            addToProgramAddressAndCycles(2,6+opCycles);
            break;
        }
        case(0x19):                 // ORA Absolute Indexed, Y
        {
            int opCycles = opCodeAddressingCrossesPageBoundary(opCode) ? 1 : 0;
            if (accumulatorIs8BitWide()) {
                executeORA8Bit(opCode);
                addToProgramAddressAndCycles(3,4+opCycles);
            } else {
                executeORA16Bit(opCode);
                addToProgramAddressAndCycles(3,5+opCycles);
            }
            break;
        }
        case(0x1D):                 // ORA Absolute Indexed, X
        {
            int opCycles = opCodeAddressingCrossesPageBoundary(opCode) ? 1 : 0;
            if (accumulatorIs8BitWide()) {
                executeORA8Bit(opCode);
                addToProgramAddressAndCycles(3,4+opCycles);
            } else {
                executeORA16Bit(opCode);
                addToProgramAddressAndCycles(3,5+opCycles);
            }
            break;
        }
        case(0x1F):                 // ORA Absolute Long Indexed, X
        {
            if (accumulatorIs8BitWide()) {
                executeORA8Bit(opCode);
                addToProgramAddressAndCycles(4,5);
            } else {
                executeORA16Bit(opCode);
                addToProgramAddressAndCycles(4,6);
            }
            break;
        }
    }
}

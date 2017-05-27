#include "Cpu.hpp"

#include <cmath>

#define LOG_TAG "Cpu::executeORA"

/**
 * This file contains implementations for all ORA OpCodes.
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
    if (accumulatorIs8BitWide()) {
        executeORA8Bit(opCode);
    } else {
        executeORA16Bit(opCode);
        addToCycles(1);
    }

    switch (opCode.getCode()) {
        case (0x09):                // ORA Immediate
        {
            if (accumulatorIs16BitWide()) {
                addToProgramAddress(1);
            }
            addToProgramAddressAndCycles(2, 2);
            break;
        }
        case (0x0D):                // ORA Absolute
        {
            addToProgramAddressAndCycles(3, 4);
            break;
        }
        case (0x0F):                // ORA Absolute Long
        {
            addToProgramAddressAndCycles(4, 5);
            break;
        }
        case(0x05):                 // ORA Direct Page
        {
            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }
            addToProgramAddressAndCycles(2, 3);
            break;
        }
        case(0x12):                 // ORA Direct Page Indirect
        {
            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }
            addToProgramAddressAndCycles(2, 5);
            break;
        }
        case(0x07):                 // ORA Direct Page Indirect Long
        {
            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }
            addToProgramAddressAndCycles(2, 6);
            break;
        }
        case(0x1D):                 // ORA Absolute Indexed, X
        {
            if (opCodeAddressingCrossesPageBoundary(opCode)) {
                addToCycles(1);
            }
            addToProgramAddressAndCycles(3, 4);
            break;
        }
        case(0x1F):                 // ORA Absolute Long Indexed, X
        {
            addToProgramAddressAndCycles(4, 5);
            break;
        }
        case(0x19):                 // ORA Absolute Indexed, Y
        {
            if (opCodeAddressingCrossesPageBoundary(opCode)) {
                addToCycles(1);
            }
            addToProgramAddressAndCycles(3, 4);
            break;
        }
        case(0x15):                 // ORA Direct Page Indexed, X
        {
            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }
            addToProgramAddressAndCycles(2, 4);
            break;
        }
        case (0x01):                // ORA Direct Page Indexed Indirect, X
        {
            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }
            addToProgramAddressAndCycles(2, 6);
            break;
        }
        case(0x11):                 // ORA Direct Page Indirect Indexed, Y
        {
            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }
            if (opCodeAddressingCrossesPageBoundary(opCode)) {
                addToCycles(1);
            }
            addToProgramAddressAndCycles(2, 5);
            break;
        }
        case(0x17):                 // ORA Direct Page Indirect Long Indexed, Y
        {
            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }
            addToProgramAddressAndCycles(2, 6);
            break;
        }
        case(0x03):                // ORA Stack Relative
        {
            addToProgramAddressAndCycles(2, 4);
            break;
        }
        case(0x13):                 // Stack Relative Indirect Indexed, Y
        {
            addToProgramAddressAndCycles(2, 7);
            break;
        }
        default:
        {
            LOG_UNEXPECTED_OPCODE(opCode);
        }
    }
}

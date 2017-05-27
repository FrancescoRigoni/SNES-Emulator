#include "Cpu.hpp"

#define LOG_TAG "Cpu::executeASL"

#define DO_ASL_8_BIT(value) {                                   \
    bool newCarry = value & 0x80;                               \
    value = value << 1;                                         \
    if (newCarry) mCpuStatus.setCarryFlag();                    \
    else mCpuStatus.clearCarryFlag();                           \
    mCpuStatus.updateSignAndZeroFlagFrom8BitValue(value);       \
}

#define DO_ASL_16_BIT(value) {                                  \
    bool newCarry = value & 0x8000;                             \
    value = value << 1;                                         \
    if (newCarry) mCpuStatus.setCarryFlag();                    \
    else mCpuStatus.clearCarryFlag();                           \
    mCpuStatus.updateSignAndZeroFlagFrom16BitValue(value);      \
}

/**
 * This file contains implementations for all ASL OpCodes.
 */

void Cpu::executeMemoryASL(OpCode &opCode) {
    Address opCodeDataAddress = getAddressOfOpCodeData(opCode);

    if(accumulatorIs8BitWide()) {
        uint8_t value = mMemoryMapper.readByte(opCodeDataAddress);
        DO_ASL_8_BIT(value);
        mMemoryMapper.storeByte(opCodeDataAddress, value);
    } else {
        uint16_t value = mMemoryMapper.readTwoBytes(opCodeDataAddress);
        DO_ASL_16_BIT(value);
        mMemoryMapper.storeTwoBytes(opCodeDataAddress, value);
    }
}

void Cpu::executeAccumulatorASL(OpCode &opCode) {
    if(accumulatorIs8BitWide()) {
        uint8_t value = Binary::lower8BitsOf(mA);
        DO_ASL_8_BIT(value);
        Binary::setLower8BitsOf16BitsValue(&mA, value);
    } else {
        DO_ASL_16_BIT(mA);
    }
}

void Cpu::executeASL(OpCode &opCode) {
    switch (opCode.getCode()) {
        case (0x0A):                // ASL Accumulator
        {
            executeAccumulatorASL(opCode);
            addToProgramAddressAndCycles(1, 2);
            break;
        }
        case (0x0E):                // ASL Absolute
        {
            if (accumulatorIs16BitWide()) {
                addToCycles(2);
            }

            executeMemoryASL(opCode);
            addToProgramAddressAndCycles(3, 6);
            break;
        }
        case (0x06):                // ASL Direct Page
        {
            if (accumulatorIs16BitWide()) {
                addToCycles(2);
            }
            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }

            executeMemoryASL(opCode);
            addToProgramAddressAndCycles(2, 5);
            break;
        }
        case (0x1E):                // ASL Absolute Indexed, X
        {
            if (accumulatorIs16BitWide()) {
                addToCycles(2);
            }
#ifdef EMU_65C02
            if (!opCodeAddressingCrossesPageBoundary()) {
                subtractFromCycles(1);
            }
#endif
            executeMemoryASL(opCode);
            addToProgramAddressAndCycles(3, 7);
            break;
        }
        case (0x16):                // ASL Direct Page Indexed, X
        {
            if (accumulatorIs16BitWide()) {
                addToCycles(2);
            }
            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }

            executeMemoryASL(opCode);
            addToProgramAddressAndCycles(2, 6);
            break;
        }
        default:
        {
            LOG_UNEXPECTED_OPCODE(opCode);
        }
    }
}

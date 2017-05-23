#include "Cpu.hpp"

#define LOG_TAG "Cpu::executeROR"

#define DO_ROR_8_BIT(value) {                                   \
    bool carryWasSet = mCpuStatus.carryFlag();                  \
    bool carryWillBeSet = (value & 0x01) != 0;                  \
    value = value >> 1;                                         \
    if (carryWasSet) Binary::setBitIn8BitValue(&value, 7);      \
    else Binary::clearBitIn8BitValue(&value, 7);                \
    if (carryWillBeSet) mCpuStatus.setCarryFlag();              \
    else mCpuStatus.clearCarryFlag();                           \
    mCpuStatus.updateSignAndZeroFlagFrom8BitValue(value);       \
}

#define DO_ROR_16_BIT(value) {                                  \
    bool carryWasSet = mCpuStatus.carryFlag();                  \
    bool carryWillBeSet = (value & 0x0001) != 0;                \
    value = value >> 1;                                         \
    if (carryWasSet) Binary::setBitIn16BitValue(&value, 15);    \
    else Binary::clearBitIn16BitValue(&value, 15);              \
    if (carryWillBeSet) mCpuStatus.setCarryFlag();              \
    else mCpuStatus.clearCarryFlag();                           \
    mCpuStatus.updateSignAndZeroFlagFrom16BitValue(value);      \
}

/**
 * This file contains implementations for all ROR OpCodes.
 */
void Cpu::executeMemoryROR(OpCode &opCode) {
    Address opCodeDataAddress = getAddressOfOpCodeData(opCode);

    if(accumulatorIs8BitWide()) {
        uint8_t value = mMemoryMapper.readByte(opCodeDataAddress);
        DO_ROR_8_BIT(value);
        mMemoryMapper.storeByte(opCodeDataAddress, value);
    } else {
        uint16_t value = mMemoryMapper.readTwoBytes(opCodeDataAddress);
        DO_ROR_16_BIT(value);
        mMemoryMapper.storeTwoBytes(opCodeDataAddress, value);
    }
}

void Cpu::executeAccumulatorROR(OpCode &opCode) {
    if(accumulatorIs8BitWide()) {
        uint8_t value = Binary::lower8BitsOf(mA);
        DO_ROR_8_BIT(value);
        Binary::setLower8BitsOf16BitsValue(&mA, value);
    } else {
        uint16_t value = mA;
        DO_ROR_16_BIT(value);
        mA = value;
    }
}

void Cpu::executeROR(OpCode &opCode) {
    switch (opCode.getCode()) {
        case (0x6A):                // ROR accumulator
        {
            executeAccumulatorROR(opCode);
            addToProgramAddressAndCycles(1, 2);
            break;
        }
        case (0x6E):                // ROR #addr
        {
            executeMemoryROR(opCode);
            if (accumulatorIs8BitWide()) {
                addToProgramAddressAndCycles(3, 6);
            } else {
                addToProgramAddressAndCycles(3, 8);
            }
            break;
        }
        case (0x66):                // ROR Direct Page
        {
            executeMemoryROR(opCode);
            int opCycles = Binary::lower8BitsOf(mD) != 0 ? 1 : 0;
            if (accumulatorIs8BitWide()) {
                addToProgramAddressAndCycles(2, 5+opCycles);
            } else {
                addToProgramAddressAndCycles(2, 7+opCycles);
            }
            break;
        }
        case (0x7E):                // ROR Absolute Indexed, X
        {
            executeMemoryROR(opCode);
#ifdef EMU_65C02
            short opCycles = opCodeAddressingCrossesPageBoundary(opCode) ? 0 : -1;
#else
            short opCycles = 0;
#endif
            if (accumulatorIs8BitWide()) {
                addToProgramAddressAndCycles(3, 7+opCycles);
            } else {
                addToProgramAddressAndCycles(3, 9+opCycles);
            }
            break;
        }
        case (0x76):                // ROR Direct Page Indexed, X
        {
            executeMemoryROR(opCode);
            int opCycles = Binary::lower8BitsOf(mD) != 0 ? 1 : 0;
            if (accumulatorIs8BitWide()) {
                addToProgramAddressAndCycles(2, 6+opCycles);
            } else {
                addToProgramAddressAndCycles(2, 8+opCycles);
            }
            break;
        }
        default:
        {
            LOG_UNEXPECTED_OPCODE(opCode);
        }
    }
}

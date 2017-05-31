#include "Cpu.hpp"

#define LOG_TAG "Cpu::executeADC"

/**
 * This file contains the implementation for all ADC OpCodes.
 */

void Cpu::execute8BitADC(OpCode &opCode) {
    Address dataAddress = getAddressOfOpCodeData(opCode);
    uint8_t value = mMemoryMapper.readByte(dataAddress);
    uint8_t accumulator = Binary::lower8BitsOf(mA);
    uint8_t carryValue = mCpuStatus.carryFlag() ? 1 : 0;

    uint16_t result16Bit = accumulator + value + carryValue;

    // Is there a carry out of the penultimate bit, redo the sum with 7 bits value and find out.
    accumulator &= 0x7F;
    value &= 0x7F;
    uint8_t partialResult = accumulator + value + carryValue;
    // Is bit 8 set?
    bool carryOutOfPenultimateBit = partialResult & 0x80;

    // Is there a carry out of the last bit, check bit 8 for that?
    bool carryOutOfLastBit = result16Bit & 0x0100;

    bool overflow = carryOutOfLastBit ^ carryOutOfPenultimateBit;
    if (overflow) mCpuStatus.setOverflowFlag();
    else mCpuStatus.clearOverflowFlag();

    if (carryOutOfLastBit) mCpuStatus.setCarryFlag();
    else mCpuStatus.clearCarryFlag();

    uint8_t result8Bit = Binary::lower8BitsOf(result16Bit);
    // Update sign and zero flags
    mCpuStatus.updateSignAndZeroFlagFrom8BitValue(result8Bit);
    // Store the 8 bit result in the accumulator
    Binary::setLower8BitsOf16BitsValue(&mA, result8Bit);
}

void Cpu::execute16BitADC(OpCode &opCode) {
    Address dataAddress = getAddressOfOpCodeData(opCode);
    uint16_t value = mMemoryMapper.readTwoBytes(dataAddress);
    uint16_t accumulator = mA;
    uint16_t carryValue = mCpuStatus.carryFlag() ? 1 : 0;

    uint32_t result32Bit = accumulator + value + carryValue;

    // Is there a carry out of the penultimate bit, redo the sum with 15 bits value and find out.
    accumulator &= 0x7FFF;
    value &= 0x7FFF;
    uint16_t partialResult = accumulator + value + carryValue;
    // Is bit 8 set?
    bool carryOutOfPenultimateBit = partialResult & 0x8000;

    // Is there a carry out of the last bit, check bit 16 for that?
    bool carryOutOfLastBit = result32Bit & 0x010000;

    bool overflow = carryOutOfLastBit ^ carryOutOfPenultimateBit;
    if (overflow) mCpuStatus.setOverflowFlag();
    else mCpuStatus.clearOverflowFlag();

    if (carryOutOfLastBit) mCpuStatus.setCarryFlag();
    else mCpuStatus.clearCarryFlag();

    uint8_t result16Bit = Binary::lower16BitsOf(result32Bit);
    // Update sign and zero flags
    mCpuStatus.updateSignAndZeroFlagFrom8BitValue(result16Bit);
    // Store the 16 bit result in the accumulator
    mA = result16Bit;
}

void Cpu::execute8BitBCDADC(OpCode &opCode) {
    Address dataAddress = getAddressOfOpCodeData(opCode);
    uint8_t value = mMemoryMapper.readByte(dataAddress);
    uint8_t accumulator = Binary::lower8BitsOf(mA);

    uint8_t result = 0;
    bool carry = Binary::bcdSum8Bit(value, accumulator, &result, mCpuStatus.carryFlag());
    if (carry) mCpuStatus.setCarryFlag();
    else mCpuStatus.clearCarryFlag();

    Binary::setLower8BitsOf16BitsValue(&mA, result);
    mCpuStatus.updateSignAndZeroFlagFrom8BitValue(result);
}

void Cpu::execute16BitBCDADC(OpCode &opCode) {
    Address dataAddress = getAddressOfOpCodeData(opCode);
    uint16_t value = mMemoryMapper.readTwoBytes(dataAddress);
    uint16_t accumulator = mA;

    uint16_t result = 0;
    bool carry = Binary::bcdSum16Bit(value, accumulator, &result, mCpuStatus.carryFlag());
    if (carry) mCpuStatus.setCarryFlag();
    else mCpuStatus.clearCarryFlag();

    mA = result;
    mCpuStatus.updateSignAndZeroFlagFrom8BitValue(result);
}

void Cpu::executeADC(OpCode &opCode) {
    // All OpCodes take one more cycle on 65C02 in decimal mode
#ifdef EMU_65C02
    if (mCpuStatus.decimalFlag()) {
        addToCycles(1);
    }
#endif
    switch (opCode.getCode()) {
        case(0x69):                 // ADC #const
        {
            if (accumulatorIs8BitWide()) {
                if (mCpuStatus.decimalFlag()) execute8BitBCDADC(opCode);
                else execute8BitADC(opCode);
            } else {
                if (mCpuStatus.decimalFlag()) execute16BitBCDADC(opCode);
                else execute16BitADC(opCode);
                addToProgramAddress(1);
                addToCycles(1);
            }
            addToProgramAddress(2);
            addToCycles(2);
            break;
        }
        case(0x6D):                 // ADC absolute
        {
            if (accumulatorIs8BitWide()) {
                if (mCpuStatus.decimalFlag()) execute8BitBCDADC(opCode);
                else execute8BitADC(opCode);
            } else {
                if (mCpuStatus.decimalFlag()) execute16BitBCDADC(opCode);
                else execute16BitADC(opCode);
                addToCycles(1);
            }
            addToProgramAddress(3);
            addToCycles(4);
            break;
        }
        case(0x6F):                 // ADC absolute long
        {
            if (accumulatorIs8BitWide()) {
                if (mCpuStatus.decimalFlag()) execute8BitBCDADC(opCode);
                else execute8BitADC(opCode);if (accumulatorIs8BitWide()) {
                if (mCpuStatus.decimalFlag()) execute8BitBCDADC(opCode);
                else execute8BitADC(opCode);
            } else {
                if (mCpuStatus.decimalFlag()) execute16BitBCDADC(opCode);
                else execute16BitADC(opCode);
                addToCycles(1);
            }

            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }

            addToProgramAddress(2);
            addToCycles(5);
            } else {
                if (mCpuStatus.decimalFlag()) execute16BitBCDADC(opCode);
                else execute16BitADC(opCode);
                addToCycles(1);
            }
            addToProgramAddress(4);
            addToCycles(5);
            break;
        }
        case(0x65):                 // ADC Direct Page
        {
            if (accumulatorIs8BitWide()) {
                if (mCpuStatus.decimalFlag()) execute8BitBCDADC(opCode);
                else execute8BitADC(opCode);
            } else {
                if (mCpuStatus.decimalFlag()) execute16BitBCDADC(opCode);
                else execute16BitADC(opCode);
                addToCycles(1);
            }

            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }

            addToProgramAddress(2);
            addToCycles(3);
            break;
        }
        case(0x72):                 // ADC Direct Page Indirect
        {
            if (accumulatorIs8BitWide()) {
                if (mCpuStatus.decimalFlag()) execute8BitBCDADC(opCode);
                else execute8BitADC(opCode);
            } else {
                if (mCpuStatus.decimalFlag()) execute16BitBCDADC(opCode);
                else execute16BitADC(opCode);
                addToCycles(1);
            }

            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }

            addToProgramAddress(2);
            addToCycles(5);
            break;
        }
        case(0x67):                 // ADC Direct Page Indirect Long
        {
            if (accumulatorIs8BitWide()) {
                if (mCpuStatus.decimalFlag()) execute8BitBCDADC(opCode);
                else execute8BitADC(opCode);
            } else {
                if (mCpuStatus.decimalFlag()) execute16BitBCDADC(opCode);
                else execute16BitADC(opCode);
                addToCycles(1);
            }

            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }

            addToProgramAddress(2);
            addToCycles(6);
            break;
        }
        case(0x7D):                 // ADC Absolute Indexed, X
        {
            if (accumulatorIs8BitWide()) {
                if (mCpuStatus.decimalFlag()) execute8BitBCDADC(opCode);
                else execute8BitADC(opCode);
            } else {
                if (mCpuStatus.decimalFlag()) execute16BitBCDADC(opCode);
                else execute16BitADC(opCode);
                addToCycles(1);
            }

            if (opCodeAddressingCrossesPageBoundary(opCode)) {
                addToCycles(1);
            }

            addToProgramAddress(3);
            addToCycles(4);
            break;
        }
        case(0x7F):                 // ADC Absolute Long Indexed, X
        {
            if (accumulatorIs8BitWide()) {
                if (mCpuStatus.decimalFlag()) execute8BitBCDADC(opCode);
                else execute8BitADC(opCode);
            } else {
                if (mCpuStatus.decimalFlag()) execute16BitBCDADC(opCode);
                else execute16BitADC(opCode);
                addToCycles(1);
            }

            addToProgramAddress(4);
            addToCycles(5);
            break;
        }
        case(0x79):                 // ADC Absolute Indexed Y
        {
            if (accumulatorIs8BitWide()) {
                if (mCpuStatus.decimalFlag()) execute8BitBCDADC(opCode);
                else execute8BitADC(opCode);
            } else {
                if (mCpuStatus.decimalFlag()) execute16BitBCDADC(opCode);
                else execute16BitADC(opCode);
                addToCycles(1);
            }

            if (opCodeAddressingCrossesPageBoundary(opCode)) {
                addToCycles(1);
            }

            addToProgramAddress(3);
            addToCycles(4);
            break;
        }
        case(0x75):                 // ADC Direct Page Indexed, X
        {
            if (accumulatorIs8BitWide()) {
                if (mCpuStatus.decimalFlag()) execute8BitBCDADC(opCode);
                else execute8BitADC(opCode);
            } else {
                if (mCpuStatus.decimalFlag()) execute16BitBCDADC(opCode);
                else execute16BitADC(opCode);
                addToCycles(1);
            }

            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }

            addToProgramAddress(2);
            addToCycles(4);
            break;
        }
        case(0x61):                 // ADC Direct Page Indexed Indirect, X
        {
            if (accumulatorIs8BitWide()) {
                if (mCpuStatus.decimalFlag()) execute8BitBCDADC(opCode);
                else execute8BitADC(opCode);
            } else {
                if (mCpuStatus.decimalFlag()) execute16BitBCDADC(opCode);
                else execute16BitADC(opCode);
                addToCycles(1);
            }

            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }

            addToProgramAddress(2);
            addToCycles(6);
            break;
        }
        case(0x71):                 // ADC Direct Page Indirect Indexed, Y
        {
            if (accumulatorIs8BitWide()) {
                if (mCpuStatus.decimalFlag()) execute8BitBCDADC(opCode);
                else execute8BitADC(opCode);
            } else {
                if (mCpuStatus.decimalFlag()) execute16BitBCDADC(opCode);
                else execute16BitADC(opCode);
                addToCycles(1);
            }

            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }
            if (opCodeAddressingCrossesPageBoundary(opCode)) {
                addToCycles(1);
            }

            addToProgramAddress(2);
            addToCycles(5);
            break;
        }
        case(0x77):                 // ADC Direct Page Indirect Long Indexed, Y
        {
            if (accumulatorIs8BitWide()) {
                if (mCpuStatus.decimalFlag()) execute8BitBCDADC(opCode);
                else execute8BitADC(opCode);
            } else {
                if (mCpuStatus.decimalFlag()) execute16BitBCDADC(opCode);
                else execute16BitADC(opCode);
                addToCycles(1);
            }

            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }

            addToProgramAddress(2);
            addToCycles(6);
            break;
        }
        case(0x63):                 // ADC Stack Relative
        {
            if (accumulatorIs8BitWide()) {
                if (mCpuStatus.decimalFlag()) execute8BitBCDADC(opCode);
                else execute8BitADC(opCode);
            } else {
                if (mCpuStatus.decimalFlag()) execute16BitBCDADC(opCode);
                else execute16BitADC(opCode);
                addToCycles(1);
            }

            addToProgramAddress(2);
            addToCycles(4);
            break;
        }
        case(0x73):                 // ADC Stack Relative Indirect Indexed, Y
        {
            if (accumulatorIs8BitWide()) {
                if (mCpuStatus.decimalFlag()) execute8BitBCDADC(opCode);
                else execute8BitADC(opCode);
            } else {
                if (mCpuStatus.decimalFlag()) execute16BitBCDADC(opCode);
                else execute16BitADC(opCode);
                addToCycles(1);
            }

            addToProgramAddress(2);
            addToCycles(7);
            break;
        }
        default: {
            LOG_UNEXPECTED_OPCODE(opCode);
        }
    }
}
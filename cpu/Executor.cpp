
#include "Cpu65816.hpp"

#include <cmath>

#define LOG_TAG "Cpu"

bool Cpu65816::executeNext() {
    // Fetch the instruction
    const uint8_t instruction = mMemoryMapper.readByte(mProgramAddress);
    OpCode opCode = OP_CODE_TABLE[instruction];
    // Log the OpCode
    trace(opCode);
    // TODO: make decent debugger
    if (mBreakpointEnabled) {
        if (mProgramAddress.getBank() == mBreakBank && mProgramAddress.getOffset() == mBreakOffset) return false;
    }
    // Execute it
    if (opCode.execute(*this)) {
        return true;
    } else {
        return useDeprecatedExecutor(opCode);
    }
}

bool Cpu65816::useDeprecatedExecutor(OpCode &opCode) {
    // Fetch address of OpCode data
    Address opCodeDataAddress = getAddressOfOpCodeData(opCode);
    switch (opCode.getCode()) {




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
*/
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




        * */

        default:
            Log::trc(LOG_TAG).str("Unimplemented!").show();
            return false;
            break;
    }

    return true;
}

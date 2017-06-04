
#include "Cpu65816.hpp"

#define LOG_TAG "Addressing"

bool Cpu65816::opCodeAddressingCrossesPageBoundary(OpCode &opCode) {
    switch(opCode.getAddressingMode()) {
        case AbsoluteIndexedWithX:
        {
            Address initialAddress(mDB, mMemoryMapper.readTwoBytes(mProgramAddress.newWithOffset(1)));
            // TODO: figure out when to wrap around and when not to, it should not matter in this case
            // but it matters when fetching data
            Address finalAddress = Address::sumOffsetToAddress(initialAddress, indexWithXRegister());
            return Address::offsetsAreOnDifferentPages(initialAddress.getOffset(), finalAddress.getOffset());
        }
        case AbsoluteIndexedWithY:
        {
            Address initialAddress(mDB, mMemoryMapper.readTwoBytes(mProgramAddress.newWithOffset(1)));
            // TODO: figure out when to wrap around and when not to, it should not matter in this case
            // but it matters when fetching data
            Address finalAddress = Address::sumOffsetToAddress(initialAddress, indexWithYRegister());
            return Address::offsetsAreOnDifferentPages(initialAddress.getOffset(), finalAddress.getOffset());
        }
        case DirectPageIndirectIndexedWithY:
        {
            uint16_t firstStageOffset = mD + mMemoryMapper.readByte(mProgramAddress.newWithOffset(1));
            Address firstStageAddress(0x00, firstStageOffset);
            uint16_t secondStageOffset = mMemoryMapper.readTwoBytes(firstStageAddress);
            Address thirdStageAddress(mDB, secondStageOffset);
            // TODO: figure out when to wrap around and when not to, it should not matter in this case
            // but it matters when fetching data
            Address finalAddress = Address::sumOffsetToAddress(thirdStageAddress, indexWithYRegister());
            return Address::offsetsAreOnDifferentPages(thirdStageAddress.getOffset(), finalAddress.getOffset());
        }
        default:
        {
            Log::err(LOG_TAG).str("!!! Unsupported opCodeAddressingCrossesPageBoundary for opCode: ").hex(opCode.getCode(), 2).show();
        }

    }

    return false;
}

Address Cpu65816::getAddressOfOpCodeData(OpCode &opCode) {
    uint8_t dataAddressBank;
    uint16_t dataAddressOffset;

    switch(opCode.getAddressingMode()) {
        case Interrupt:
        case Accumulator:
        case Implied:
        case StackImplied:
            // Not really used, doesn't make any sense since these opcodes do not have operands
            return mProgramAddress;
        case Immediate:
        case BlockMove:
            // Blockmove OpCodes have two bytes following them directly
        case StackAbsolute:
            // Stack absolute is used to push values following the op code onto the stack
        case ProgramCounterRelative:
            // Program counter relative OpCodes such as all branch instructions have an 8 bit operand
            // following the op code
        case ProgramCounterRelativeLong:
            // StackProgramCounterRelativeLong is only used by the PER OpCode, it has 16 bit operand
        case StackProgramCounterRelativeLong:
            mProgramAddress.newWithOffset(1).getBankAndOffset(&dataAddressBank, &dataAddressOffset);
            break;
        case Absolute:
            dataAddressBank = mDB;
            dataAddressOffset = mMemoryMapper.readTwoBytes(mProgramAddress.newWithOffset(1));
            break;
        case AbsoluteLong:
            mMemoryMapper.readAddressAt(mProgramAddress.newWithOffset(1)).getBankAndOffset(&dataAddressBank, &dataAddressOffset);
            break;
        case AbsoluteIndirect:
        {
            dataAddressBank = mProgramAddress.getBank();
            Address addressOfOffset(0x00, mMemoryMapper.readTwoBytes(mProgramAddress.newWithOffset(1)));
            dataAddressOffset = mMemoryMapper.readTwoBytes(addressOfOffset);
        }
            break;
        case AbsoluteIndirectLong:
        {
            Address addressOfEffectiveAddress(0x00, mMemoryMapper.readTwoBytes(mProgramAddress.newWithOffset(1)));
            mMemoryMapper.readAddressAt(addressOfEffectiveAddress).getBankAndOffset(&dataAddressBank, &dataAddressOffset);
        }
            break;
        case AbsoluteIndexedIndirectWithX:
        {
            Address firstStageAddress(mProgramAddress.getBank(), mMemoryMapper.readTwoBytes(mProgramAddress.newWithOffset(1)));
            Address secondStageAddress = firstStageAddress.newWithOffsetNoWrapAround(indexWithXRegister());
            dataAddressBank = mProgramAddress.getBank();
            dataAddressOffset = mMemoryMapper.readTwoBytes(secondStageAddress);
        }
            break;
        case AbsoluteIndexedWithX:
        {
            Address firstStageAddress(mDB, mMemoryMapper.readTwoBytes(mProgramAddress.newWithOffset(1)));
            Address::sumOffsetToAddressNoWrapAround(firstStageAddress, indexWithXRegister())
                .getBankAndOffset(&dataAddressBank, &dataAddressOffset);;
        }
            break;
        case AbsoluteLongIndexedWithX:
        {
            Address firstStageAddress = mMemoryMapper.readAddressAt(mProgramAddress.newWithOffset(1));
            Address::sumOffsetToAddressNoWrapAround(firstStageAddress, indexWithXRegister())
                .getBankAndOffset(&dataAddressBank, &dataAddressOffset);;
        }
            break;
        case AbsoluteIndexedWithY:
        {
            Address firstStageAddress(mDB, mMemoryMapper.readTwoBytes(mProgramAddress.newWithOffset(1)));
            Address::sumOffsetToAddressNoWrapAround(firstStageAddress, indexWithYRegister())
                .getBankAndOffset(&dataAddressBank, &dataAddressOffset);;
        }
            break;
        case DirectPage:
        {
            // Direct page/Zero page always refers to bank zero
            dataAddressBank = 0x00;
            if (mCpuStatus.emulationFlag()) {
                // 6502 uses zero page
                dataAddressOffset = mMemoryMapper.readByte(mProgramAddress.newWithOffset(1));
            } else {
                // 65816 uses direct page
                dataAddressOffset = mD + mMemoryMapper.readByte(mProgramAddress.newWithOffset(1));
            }
        }
            break;
        case DirectPageIndexedWithX:
        {
            dataAddressBank = 0x00;
            dataAddressOffset = mD + indexWithXRegister() + mMemoryMapper.readByte(mProgramAddress.newWithOffset(1));
        }
            break;
        case DirectPageIndexedWithY:
        {
            dataAddressBank = 0x00;
            dataAddressOffset = mD + indexWithYRegister() + mMemoryMapper.readByte(mProgramAddress.newWithOffset(1));
        }
            break;
        case DirectPageIndirect:
        {
            Address firstStageAddress(0x00, mD + mMemoryMapper.readByte(mProgramAddress.newWithOffset(1)));
            dataAddressBank = mDB;
            dataAddressOffset = mMemoryMapper.readTwoBytes(firstStageAddress);
        }
            break;
        case DirectPageIndirectLong:
        {
            Address firstStageAddress(0x00, mD + mMemoryMapper.readByte(mProgramAddress.newWithOffset(1)));
            mMemoryMapper.readAddressAt(firstStageAddress)
                .getBankAndOffset(&dataAddressBank, &dataAddressOffset);;
        }
            break;
        case DirectPageIndexedIndirectWithX:
        {
            Address firstStageAddress(0x00, mD + mMemoryMapper.readByte(mProgramAddress.newWithOffset(1)) + indexWithXRegister());
            dataAddressBank = mDB;
            dataAddressOffset = mMemoryMapper.readTwoBytes(firstStageAddress);
        }
            break;
        case DirectPageIndirectIndexedWithY:
        {
            Address firstStageAddress(0x00, mD + mMemoryMapper.readByte(mProgramAddress.newWithOffset(1)));
            uint16_t secondStageOffset = mMemoryMapper.readTwoBytes(firstStageAddress);
            Address thirdStageAddress(mDB, secondStageOffset);
            Address::sumOffsetToAddressNoWrapAround(thirdStageAddress, indexWithYRegister())
                .getBankAndOffset(&dataAddressBank, &dataAddressOffset);
        }
            break;
        case DirectPageIndirectLongIndexedWithY:
        {
            Address firstStageAddress(0x00, mD + mMemoryMapper.readByte(mProgramAddress.newWithOffset(1)));
            Address secondStageAddress = mMemoryMapper.readAddressAt(firstStageAddress);
            Address::sumOffsetToAddressNoWrapAround(secondStageAddress, indexWithYRegister())
                .getBankAndOffset(&dataAddressBank, &dataAddressOffset);
        }
            break;
        case StackRelative:
        {
            dataAddressBank = 0x00;
            dataAddressOffset = mStack.getStackPointer() + mMemoryMapper.readByte(mProgramAddress.newWithOffset(1));
        }
            break;
        case StackDirectPageIndirect:
        {
            dataAddressBank = 0x00;
            dataAddressOffset = mD + mMemoryMapper.readByte(mProgramAddress.newWithOffset(1));
        }
            break;
        case StackRelativeIndirectIndexedWithY:
        {
            Address firstStageAddress(0x00, mStack.getStackPointer() + mMemoryMapper.readByte(mProgramAddress.newWithOffset(1)));
            uint16_t secondStageOffset = mMemoryMapper.readTwoBytes(firstStageAddress);
            Address thirdStageAddress(mDB, secondStageOffset);
            Address::sumOffsetToAddressNoWrapAround(thirdStageAddress, indexWithYRegister())
                .getBankAndOffset(&dataAddressBank, &dataAddressOffset);
        }
            break;
    }

    return Address(dataAddressBank, dataAddressOffset);
}

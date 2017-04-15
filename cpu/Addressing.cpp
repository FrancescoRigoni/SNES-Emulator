
#include "Cpu.hpp"

#define LOG_TAG "Addressing"

Address Cpu::getAddressOfOpCodeData(OpCode &opCode) {
    Address onePlusOpCodeAddress;
    onePlusOpCodeAddress.bank = mProgramAddress.bank;
    onePlusOpCodeAddress.offset = mProgramAddress.offset + 1;
    Address dataAddress;
    
    switch(opCode.getAddressingMode()) {
        case Interrupt:
        case Accumulator:
        case Implied:
        case StackImplied:
            // Not really used, doesn't make any sense since these opcodes do not have operands
            return mProgramAddress;
            break;
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
            dataAddress = onePlusOpCodeAddress;
            break;
        case Absolute:
            dataAddress.bank = mDB;
            dataAddress.offset = mMemoryMapper.readTwoBytes(onePlusOpCodeAddress);
            break;
        case AbsoluteProgram:
            dataAddress.bank = mProgramAddress.bank;
            dataAddress.offset = mMemoryMapper.readTwoBytes(onePlusOpCodeAddress);
            break;
        case AbsoluteLong:
            dataAddress = mMemoryMapper.readAddressAt(onePlusOpCodeAddress);
            break;
        case AbsoluteIndirect:
            dataAddress.bank = mProgramAddress.bank;
            dataAddress.offset = mMemoryMapper.readTwoBytes(0x00, mMemoryMapper.readTwoBytes(onePlusOpCodeAddress));
            break;
        case AbsoluteIndirectLong:
            dataAddress = mMemoryMapper.readAddressAt(0x00, mMemoryMapper.readTwoBytes(onePlusOpCodeAddress));
            break;
        case AbsoluteIndexedIndirect:
        {
            dataAddress.bank = mProgramAddress.bank;
            uint16_t operandOffset = mMemoryMapper.readTwoBytes(onePlusOpCodeAddress);
            operandOffset += indexWithXRegister();
            dataAddress.offset = mMemoryMapper.readTwoBytes(mProgramAddress.bank, operandOffset);
        }
            break;
        case AbsoluteIndexedWithX:
        {
            Address firstStageAddress;
            firstStageAddress.bank = mDB;
            firstStageAddress.offset = mMemoryMapper.readTwoBytes(onePlusOpCodeAddress);
            dataAddress = MemoryMapper::sumOffsetToAddressNoWrapAround(firstStageAddress, indexWithXRegister());
        }
            break;
        case AbsoluteLongIndexedWithX:
        {
            Address firstStageAddress = mMemoryMapper.readAddressAt(onePlusOpCodeAddress);
            dataAddress = MemoryMapper::sumOffsetToAddressNoWrapAround(firstStageAddress, indexWithXRegister());
        }
            break;
        case AbsoluteIndexedWithY:
        {
            Address firstStageAddress;
            firstStageAddress.bank = mDB;
            firstStageAddress.offset = mMemoryMapper.readTwoBytes(onePlusOpCodeAddress);
            dataAddress = MemoryMapper::sumOffsetToAddressNoWrapAround(firstStageAddress, indexWithYRegister());
        }
            break;
        case DirectPage:
        {
            dataAddress.bank = 0x00;
            dataAddress.offset = mD + mMemoryMapper.readByte(onePlusOpCodeAddress);
        }
            break;
        case DirectPageIndexedWithX:
        {
            dataAddress.bank = 0x00;
            dataAddress.offset = mD + indexWithXRegister() + mMemoryMapper.readByte(onePlusOpCodeAddress);
        }
            break;
        case DirectPageIndexedWithY:
        {
            dataAddress.bank = 0x00;
            dataAddress.offset = mD + indexWithYRegister() + mMemoryMapper.readByte(onePlusOpCodeAddress);
        }
            break;
        case DirectPageIndirect:
        {
            Address firstStageAddress;
            firstStageAddress.bank = 0x00;
            firstStageAddress.offset = mD + mMemoryMapper.readByte(onePlusOpCodeAddress);
            dataAddress.bank = mDB;
            dataAddress.offset = mMemoryMapper.readTwoBytes(firstStageAddress);
        }
            break;
        case DirectPageIndirectLong:
        {
            Address firstStageAddress;
            firstStageAddress.bank = 0x00;
            firstStageAddress.offset = mD + mMemoryMapper.readByte(onePlusOpCodeAddress);
            dataAddress = mMemoryMapper.readAddressAt(firstStageAddress);
        }
            break;
        case DirectPageIndexedIndirectWithX:
        {
            Address firstStageAddress;
            firstStageAddress.bank = 0x00;
            firstStageAddress.offset = mD + mMemoryMapper.readByte(onePlusOpCodeAddress) + indexWithXRegister();
            dataAddress.bank = mDB;
            dataAddress.offset = mMemoryMapper.readTwoBytes(firstStageAddress);
        }
            break;
        case DirectPageIndirectIndexedWithY:
        {
            uint16_t firstStageOffset = mD + mMemoryMapper.readByte(onePlusOpCodeAddress);
            uint16_t secondStageOffset = mMemoryMapper.readTwoBytes(0x00, firstStageOffset);
            Address thirdStageAddress;
            thirdStageAddress.bank = mDB;
            thirdStageAddress.offset = secondStageOffset;
            dataAddress = MemoryMapper::sumOffsetToAddressNoWrapAround(thirdStageAddress, indexWithXRegister());
        }
            break;
        case DirectPageIndirectLongIndexedWithY:
        {
            uint16_t firstStageOffset = mD + mMemoryMapper.readByte(onePlusOpCodeAddress);
            Address secondStageAddress = mMemoryMapper.readAddressAt(0x00, firstStageOffset);
            dataAddress = MemoryMapper::sumOffsetToAddressNoWrapAround(secondStageAddress, indexWithYRegister());
        }
            break;
        case StackRelative:
        {
            dataAddress.bank = 0x00;
            dataAddress.offset = mStack.getStackPointer() + mMemoryMapper.readByte(onePlusOpCodeAddress);
        }
            break;
        case StackDirectPageIndirect:
        {
            dataAddress.bank = 0x00;
            dataAddress.offset = mD + mMemoryMapper.readByte(onePlusOpCodeAddress);
        }
            break;
        case StackRelativeIndirectIndexedWithY:
        {
            uint16_t firstStageOffset = mStack.getStackPointer() + mMemoryMapper.readByte(onePlusOpCodeAddress);
            uint16_t secondStageOffset = mMemoryMapper.readTwoBytes(0x00, firstStageOffset);
            Address thirdStageAddress;
            thirdStageAddress.bank = mDB;
            thirdStageAddress.offset = secondStageOffset;
            dataAddress = MemoryMapper::sumOffsetToAddressNoWrapAround(thirdStageAddress, indexWithYRegister());
        }
            break;
    }
    
    return dataAddress;
}

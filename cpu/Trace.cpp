#include "Log.hpp"
#include "Cpu65816.hpp"

#define LOG_TAG "Trace"

void Cpu65816::trace(OpCode &opCode) {
    Address onePlusOpCodeAddress = mProgramAddress.newWithOffset(1);

    Log &log = Log::trc(LOG_TAG);
    log.hex(mProgramAddress.getBank(), 2).str(":").hex(mProgramAddress.getOffset(), 4);
    log.str(" | ").hex(opCode.getCode(), 2).sp().str(opCode.getName()).sp();

    switch(opCode.getAddressingMode()) {
        case AddressingMode::Interrupt:
        case AddressingMode::Accumulator:
        case AddressingMode::Implied:
            break;
        case AddressingMode::Immediate:
            // This refers to accumulator size to estimate the kind of value to print.
            // Instructions using index registers might print the wrong value.
            if (accumulatorIs8BitWide()) {
                log.str("#").hex(mMemoryMapper.readByte(getAddressOfOpCodeData(opCode)), 2);
            } else {
                log.str("#").hex(mMemoryMapper.readTwoBytes(getAddressOfOpCodeData(opCode)), 4);
            }
            break;
        case AddressingMode::Absolute:
            log.hex(getAddressOfOpCodeData(opCode).getOffset(), 4).sp();
            log.str("                    [Absolute]");
            break;
        case AddressingMode::AbsoluteLong:
        {
            Address opCodeDataAddress = getAddressOfOpCodeData(opCode);
            log.hex(opCodeDataAddress.getBank(), 2).str(":").hex(opCodeDataAddress.getOffset(), 4).sp();
            log.str("                [Absolute Long]");
        }
            break;
        case AddressingMode::AbsoluteIndirect:
            break;
        case AddressingMode::AbsoluteIndirectLong:
            break;
        case AddressingMode::AbsoluteIndexedIndirectWithX:
            break;
        case AddressingMode::AbsoluteIndexedWithX:
            log.hex(mMemoryMapper.readTwoBytes(onePlusOpCodeAddress), 4).str(", X").sp();
            log.str("                 [Absolute Indexed, X]");
            break;
        case AddressingMode::AbsoluteLongIndexedWithX:
        {
            Address opCodeDataAddress = getAddressOfOpCodeData(opCode);
            Address effectiveAddress = mMemoryMapper.readAddressAt(opCodeDataAddress);
            log.hex(effectiveAddress.getBank(), 2).str(":").hex(effectiveAddress.getOffset(), 4).str(", X").sp();
            log.str("                    [Absolute Long Indexed, X]");
        }
            break;
        case AddressingMode::AbsoluteIndexedWithY:
            log.hex(mMemoryMapper.readTwoBytes(getAddressOfOpCodeData(opCode)), 4).str(", Y").sp();
            log.str("                    [Absolute Indexed, Y]");
            break;
        case AddressingMode::DirectPage:
            log.hex(mMemoryMapper.readByte(onePlusOpCodeAddress), 2).sp();
            log.str("                      [Direct Page]");
            break;
        case AddressingMode::DirectPageIndexedWithX:
            log.hex(mMemoryMapper.readByte(onePlusOpCodeAddress), 2).str(", X").sp();
            log.str("                    [Direct Page Indexed, X]");
            break;
        case AddressingMode::DirectPageIndexedWithY:
            log.hex(mMemoryMapper.readByte(onePlusOpCodeAddress), 2).str(", Y").sp();
            log.str("                    [Direct Page Indexed, Y]");
            break;
        case AddressingMode::DirectPageIndirect:
            log.str("(").hex(mMemoryMapper.readByte(onePlusOpCodeAddress), 2).str(")").sp();
            log.str("                    [Direct Page Indirect]");
            break;
        case AddressingMode::DirectPageIndirectLong:
            log.str("[").hex(mMemoryMapper.readByte(getAddressOfOpCodeData(opCode)), 2).str("]").sp();
            log.str("                    [Direct Page Indirect Long]");
            break;
        case AddressingMode::DirectPageIndexedIndirectWithX:
            log.str("(").hex(mMemoryMapper.readByte(getAddressOfOpCodeData(opCode)), 2).str(", X)").sp();
            log.str("                    [Direct Page Indexed Indirect, X]");
            break;
        case AddressingMode::DirectPageIndirectIndexedWithY:
            log.str("(").hex(mMemoryMapper.readByte(getAddressOfOpCodeData(opCode)), 2).str("), Y").sp();
            log.str("                    [Direct Page Indirect Indexed, Y]");
            break;
        case AddressingMode::DirectPageIndirectLongIndexedWithY:
            log.str("[").hex(mMemoryMapper.readByte(getAddressOfOpCodeData(opCode)), 2).str("], Y").sp();
            log.str("                    [Direct Page Indirect Indexed, Y]");
            break;
        case AddressingMode::StackImplied:
            log.str("                          [Stack Implied]");
            break;
        case AddressingMode::StackRelative:
            log.hex(mMemoryMapper.readByte(getAddressOfOpCodeData(opCode)), 2).str(", S").sp();
            log.str("                    [Stack Relative]");
            break;
        case AddressingMode::StackAbsolute:
            break;
        case AddressingMode::StackDirectPageIndirect:
            break;
        case AddressingMode::StackProgramCounterRelativeLong:
            break;
        case AddressingMode::StackRelativeIndirectIndexedWithY:
            log.str("(").hex(mMemoryMapper.readByte(Address::sumOffsetToAddressWrapAround(mProgramAddress, 1)), 2);
            log.str(", S), Y").sp();
            log.str("                    [Absolute Indexed, X]");
            break;
        case AddressingMode::ProgramCounterRelative:
            log.hex(mMemoryMapper.readByte(getAddressOfOpCodeData(opCode)), 2).sp();
            log.str("                      [Program Counter Relative]");
            break;
        case AddressingMode::ProgramCounterRelativeLong:
            break;
        case AddressingMode::BlockMove:
            break;
    }

    log.show();
}

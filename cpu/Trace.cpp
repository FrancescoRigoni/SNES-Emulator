#include "Log.hpp"
#include "Cpu65816.hpp"

#define LOG_TAG "Trace"

void Cpu65816::trace(OpCode &opCode) {
    Address onePlusOpCodeAddress = mProgramAddress.newWithOffset(1);

    Log &log = Log::trc(LOG_TAG);
    log.hex(mProgramAddress.getBank(), 2).str(":").hex(mProgramAddress.getOffset(), 4);
    log.str(" | ").hex(opCode.getCode(), 2).sp().str(opCode.getName()).sp();

    switch(opCode.getAddressingMode()) {
        case Interrupt:
        case Accumulator:
        case Implied:
            break;
        case Immediate:
            log.str("#").hex(mMemoryMapper.readByte(getAddressOfOpCodeData(opCode)), 2);
            break;
        case AbsoluteProgram:
            log.hex(getAddressOfOpCodeData(opCode).getOffset(), 4).sp();
            log.str("                    [Absolute (Program)]");
            break;
        case Absolute:
            log.hex(getAddressOfOpCodeData(opCode).getOffset(), 4).sp();
            log.str("                    [Absolute]");
            break;
        case AbsoluteLong:
        {
            Address opCodeDataAddress = getAddressOfOpCodeData(opCode);
            log.hex(opCodeDataAddress.getBank(), 2).str(":").hex(opCodeDataAddress.getOffset(), 4).sp();
            log.str("                [Absolute Long]");
        }
            break;
        case AbsoluteIndirect:
            break;
        case AbsoluteIndirectLong:
            break;
        case AbsoluteIndexedIndirect:
            break;
        case AbsoluteIndexedWithX:
            log.hex(mMemoryMapper.readTwoBytes(onePlusOpCodeAddress), 4).str(", X").sp();
            log.str("                 [Absolute Indexed, X]");
            break;
        case AbsoluteLongIndexedWithX:
        {
            Address opCodeDataAddress = getAddressOfOpCodeData(opCode);
            Address effectiveAddress = mMemoryMapper.readAddressAt(opCodeDataAddress);
            log.hex(effectiveAddress.getBank(), 2).str(":").hex(effectiveAddress.getOffset(), 4).str(", X").sp();
            log.str("                    [Absolute Long Indexed, X]");
        }
            break;
        case AbsoluteIndexedWithY:
            log.hex(mMemoryMapper.readTwoBytes(getAddressOfOpCodeData(opCode)), 4).str(", Y").sp();
            log.str("                    [Absolute Indexed, Y]");
            break;
        case DirectPage:
            log.hex(mMemoryMapper.readByte(getAddressOfOpCodeData(opCode)), 2).sp();
            log.str("                      [Direct Page]");
            break;
        case DirectPageIndexedWithX:
            log.hex(mMemoryMapper.readByte(getAddressOfOpCodeData(opCode)), 2).str(", X").sp();
            log.str("                    [Direct Page Indexed, X]");
            break;
        case DirectPageIndexedWithY:
            log.hex(mMemoryMapper.readByte(getAddressOfOpCodeData(opCode)), 2).str(", Y").sp();
            log.str("                    [Direct Page Indexed, Y]");
            break;
        case DirectPageIndirect:
            log.str("(").hex(mMemoryMapper.readByte(getAddressOfOpCodeData(opCode)), 2).str(")").sp();
            log.str("                    [Direct Page Indirect]");
            break;
        case DirectPageIndirectLong:
            log.str("[").hex(mMemoryMapper.readByte(getAddressOfOpCodeData(opCode)), 2).str("]").sp();
            log.str("                    [Direct Page Indirect Long]");
            break;
        case DirectPageIndexedIndirectWithX:
            log.str("(").hex(mMemoryMapper.readByte(getAddressOfOpCodeData(opCode)), 2).str(", X)").sp();
            log.str("                    [Direct Page Indexed Indirect, X]");
            break;
        case DirectPageIndirectIndexedWithY:
            log.str("(").hex(mMemoryMapper.readByte(getAddressOfOpCodeData(opCode)), 2).str("), Y").sp();
            log.str("                    [Direct Page Indirect Indexed, Y]");
            break;
        case DirectPageIndirectLongIndexedWithY:
            log.str("[").hex(mMemoryMapper.readByte(getAddressOfOpCodeData(opCode)), 2).str("], Y").sp();
            log.str("                    [Direct Page Indirect Indexed, Y]");
            break;
        case StackImplied:
            log.str("                          [Stack Implied]");
            break;
        case StackRelative:
            log.hex(mMemoryMapper.readByte(getAddressOfOpCodeData(opCode)), 2).str(", S").sp();
            log.str("                    [Stack Relative]");
            break;
        case StackAbsolute:
            break;
        case StackDirectPageIndirect:
            break;
        case StackProgramCounterRelativeLong:
            break;
        case StackRelativeIndirectIndexedWithY:
            log.str("(").hex(mMemoryMapper.readByte(Address::sumOffsetToAddressWrapAround(mProgramAddress, 1)), 2);
            log.str(", S), Y").sp();
            log.str("                    [Absolute Indexed, X]");
            break;
        case ProgramCounterRelative:
            log.hex(mMemoryMapper.readByte(getAddressOfOpCodeData(opCode)), 2).sp();
            log.str("                      [Program Counter Relative]");
            break;
        case ProgramCounterRelativeLong:
            break;
        case BlockMove:
            break;
    }

    log.show();
}

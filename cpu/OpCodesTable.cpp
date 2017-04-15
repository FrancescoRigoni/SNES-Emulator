
#include "Cpu.hpp"
#include "Log.hpp"

#define LOG_TAG "OpCode"

OpCode::OpCode(uint8_t code, const char * const name, const AddressingMode &addressingMode) :
    mCode(code), mName(name), mAddressingMode(addressingMode) {
}

OpCode OP_CODE[] = { 
 OpCode(0x00, "BRK", Interrupt),
 OpCode(0x01, "ORA", DirectPageIndexedIndirectWithX),
 OpCode(0x02, "COP", Interrupt),
 OpCode(0x03, "ORA", StackRelative),
 OpCode(0x04, "TSB", DirectPage),
 OpCode(0x05, "ORA", DirectPage),
 OpCode(0x06, "ASL", DirectPage),
 OpCode(0x07, "ORA", DirectPageIndirectLong),
 OpCode(0x08, "PHP", StackImplied),
 OpCode(0x09, "ORA", Immediate),
 OpCode(0x0A, "ASL", Accumulator),
 OpCode(0x0B, "PHD", StackImplied),
 OpCode(0x0C, "TSB", Absolute),
 OpCode(0x0D, "ORA", Absolute),
 OpCode(0x0E, "ASL", Absolute),
 OpCode(0x0F, "ORA", AbsoluteLong),
 OpCode(0x10, "BPL", ProgramCounterRelative),
 OpCode(0x11, "ORA", DirectPageIndirectIndexedWithY),
 OpCode(0x12, "ORA", DirectPageIndirect),
 OpCode(0x13, "ORA", StackRelativeIndirectIndexedWithY),
 OpCode(0x14, "TRB", DirectPage),
 OpCode(0x15, "ORA", DirectPageIndexedWithX),
 OpCode(0x16, "ASL", DirectPageIndexedWithX),
 OpCode(0x17, "ORA", DirectPageIndirectLongIndexedWithY),
 OpCode(0x18, "CLC", Implied),
 OpCode(0x19, "ORA", AbsoluteIndexedWithY),
 OpCode(0x1A, "INC", Accumulator),
 OpCode(0x1B, "TCS", Implied),
 OpCode(0x1C, "TRB", Absolute),
 OpCode(0x1D, "ORA", AbsoluteIndexedWithX),
 OpCode(0x1E, "ASL", AbsoluteIndexedWithX),
 OpCode(0x1F, "ORA", AbsoluteLongIndexedWithX),
 OpCode(0x20, "JSR", AbsoluteProgram),
 OpCode(0x21, "AND", DirectPageIndexedIndirectWithX),
 OpCode(0x22, "JSR", AbsoluteLong),
 OpCode(0x23, "AND", StackRelative),
 OpCode(0x24, "BIT", DirectPage),
 OpCode(0x25, "AND", DirectPage),
 OpCode(0x26, "ROL", DirectPage),
 OpCode(0x27, "AND", DirectPageIndirectLong),
 OpCode(0x28, "PLP", StackImplied),
 OpCode(0x29, "AND", Immediate),
 OpCode(0x2A, "ROL", Accumulator),
 OpCode(0x2B, "PLD", StackImplied),
 OpCode(0x2C, "BIT", Absolute),
 OpCode(0x2D, "AND", Absolute),
 OpCode(0x2E, "ROL", Absolute),
 OpCode(0x2F, "AND", AbsoluteLong),
 OpCode(0x30, "BMI", ProgramCounterRelative),
 OpCode(0x31, "AND", DirectPageIndirectIndexedWithY),
 OpCode(0x32, "AND", DirectPageIndirect),
 OpCode(0x33, "AND", StackRelativeIndirectIndexedWithY),
 OpCode(0x34, "BIT", DirectPageIndexedWithX),
 OpCode(0x35, "AND", DirectPageIndexedWithX),
 OpCode(0x36, "ROL", DirectPageIndexedWithX),
 OpCode(0x37, "AND", DirectPageIndirectLongIndexedWithY),
 OpCode(0x38, "SEC", Implied),
 OpCode(0x39, "AND", AbsoluteIndexedWithY),
 OpCode(0x3A, "DEC", Accumulator),
 OpCode(0x3B, "TSC", Implied),
 OpCode(0x3C, "BIT", AbsoluteIndexedWithX),
 OpCode(0x3D, "AND", AbsoluteIndexedWithX),
 OpCode(0x3E, "ROL", AbsoluteIndexedWithX),
 OpCode(0x3F, "AND", AbsoluteLongIndexedWithX),
 OpCode(0x40, "RTI", StackImplied),
 OpCode(0x41, "EOR", DirectPageIndexedIndirectWithX),
 OpCode(0x42, "WDM", Implied),
 OpCode(0x43, "EOR", StackRelative),
 OpCode(0x44, "MVP", BlockMove),
 OpCode(0x45, "EOR", DirectPage),
 OpCode(0x46, "LSR", DirectPage),
 OpCode(0x47, "EOR", DirectPageIndirectLong),
 OpCode(0x48, "PHA", StackImplied),
 OpCode(0x49, "EOR", Immediate),
 OpCode(0x4A, "LSR", Accumulator),
 OpCode(0x4B, "PHK", StackImplied),
 OpCode(0x4C, "JMP", AbsoluteProgram),
 OpCode(0x4D, "EOR", Absolute),
 OpCode(0x4E, "LSR", Absolute),
 OpCode(0x4F, "EOR", AbsoluteLong),
 OpCode(0x50, "BVC", ProgramCounterRelative),
 OpCode(0x51, "EOR", DirectPageIndirectIndexedWithY),
 OpCode(0x52, "EOR", DirectPageIndirect),
 OpCode(0x53, "EOR", StackRelativeIndirectIndexedWithY),
 OpCode(0x54, "MVN", BlockMove),
 OpCode(0x55, "EOR", DirectPageIndexedWithX),
 OpCode(0x56, "LSR", DirectPageIndexedWithX),
 OpCode(0x57, "EOR", DirectPageIndirectLongIndexedWithY),
 OpCode(0x58, "CLI", Implied),
 OpCode(0x59, "EOR", AbsoluteIndexedWithY),
 OpCode(0x5A, "PHY", StackImplied),
 OpCode(0x5B, "TCD", Implied),
 OpCode(0x5C, "JMP", AbsoluteLong),
 OpCode(0x5D, "EOR", AbsoluteIndexedWithX),
 OpCode(0x5E, "LSR", AbsoluteIndexedWithX),
 OpCode(0x5F, "EOR", AbsoluteLongIndexedWithX),
 OpCode(0x60, "RTS", StackImplied),
 OpCode(0x61, "ADC", DirectPageIndexedIndirectWithX),
 OpCode(0x62, "PER", StackProgramCounterRelativeLong),
 OpCode(0x63, "ADC", StackRelative),
 OpCode(0x64, "STZ", DirectPage),
 OpCode(0x65, "ADC", DirectPage),
 OpCode(0x66, "ROR", DirectPage),
 OpCode(0x67, "ADC", DirectPageIndirectLong),
 OpCode(0x68, "PLA", StackImplied),
 OpCode(0x69, "ADC", Immediate),
 OpCode(0x6A, "ROR", Accumulator),
 OpCode(0x6B, "RTL", StackImplied),
 OpCode(0x6C, "JMP", AbsoluteIndirect),
 OpCode(0x6D, "ADC", Absolute),
 OpCode(0x6E, "ROR", Absolute),
 OpCode(0x6F, "ADC", AbsoluteLong),
 OpCode(0x70, "BVS", ProgramCounterRelative),
 OpCode(0x71, "ADC", DirectPageIndirectIndexedWithY),
 OpCode(0x72, "ADC", DirectPageIndirect),
 OpCode(0x73, "ADC", StackRelativeIndirectIndexedWithY),
 OpCode(0x74, "STZ", DirectPageIndexedWithX),
 OpCode(0x75, "ADC", DirectPageIndexedWithX),
 OpCode(0x76, "ROR", DirectPageIndexedWithX),
 OpCode(0x77, "ADC", DirectPageIndirectLongIndexedWithY),
 OpCode(0x78, "SEI", Implied),
 OpCode(0x79, "ADC", AbsoluteIndexedWithY),
 OpCode(0x7A, "PLY", StackImplied),
 OpCode(0x7B, "TDC", Implied),
 OpCode(0x7C, "JMP", AbsoluteIndexedIndirect),
 OpCode(0x7D, "ADC", AbsoluteIndexedWithX),
 OpCode(0x7E, "ROR", AbsoluteIndexedWithX),
 OpCode(0x7F, "ADC", AbsoluteLongIndexedWithX),
 OpCode(0x80, "BRA", ProgramCounterRelative),
 OpCode(0x81, "STA", DirectPageIndexedIndirectWithX),
 OpCode(0x82, "BRL", ProgramCounterRelativeLong),
 OpCode(0x83, "STA", StackRelative),
 OpCode(0x84, "STY", DirectPage),
 OpCode(0x85, "STA", DirectPage),
 OpCode(0x86, "STX", DirectPage),
 OpCode(0x87, "STA", DirectPageIndirectLong),
 OpCode(0x88, "DEY", Implied),
 OpCode(0x89, "BIT", Immediate),
 OpCode(0x8A, "TXA", Implied),
 OpCode(0x8B, "PHB", StackImplied),
 OpCode(0x8C, "STY", Absolute),
 OpCode(0x8D, "STA", Absolute),
 OpCode(0x8E, "STX", Absolute),
 OpCode(0x8F, "STA", AbsoluteLong),
 OpCode(0x90, "BCC", ProgramCounterRelative),
 OpCode(0x91, "STA", DirectPageIndirectIndexedWithY),
 OpCode(0x92, "STA", DirectPageIndirect),
 OpCode(0x93, "STA", StackRelativeIndirectIndexedWithY),
 OpCode(0x94, "STY", DirectPageIndexedWithX),
 OpCode(0x95, "STAX", DirectPageIndexedWithX),
 OpCode(0x96, "STX", DirectPageIndexedWithY),
 OpCode(0x97, "STA", DirectPageIndirectLongIndexedWithY),
 OpCode(0x98, "TYA", Implied),
 OpCode(0x99, "STA", AbsoluteIndexedWithY),
 OpCode(0x9A, "TXS", Implied),
 OpCode(0x9B, "TXY", Implied),
 OpCode(0x9C, "STZ", Absolute),
 OpCode(0x9D, "STA", AbsoluteIndexedWithX),
 OpCode(0x9E, "STZ", AbsoluteIndexedWithX),
 OpCode(0x9F, "STA", AbsoluteLongIndexedWithX),
 OpCode(0xA0, "LDY", Immediate),
 OpCode(0xA1, "LDA", DirectPageIndexedIndirectWithX),
 OpCode(0xA2, "LDX", Immediate),
 OpCode(0xA3, "LDA", StackRelative),
 OpCode(0xA4, "LDY", DirectPage),
 OpCode(0xA5, "LDA", DirectPage),
 OpCode(0xA6, "LDX", DirectPage),
 OpCode(0xA7, "LDA", DirectPageIndirectLong),
 OpCode(0xA8, "TAY", Implied),
 OpCode(0xA9, "LDA", Immediate),
 OpCode(0xAA, "TAX", Implied),
 OpCode(0xAB, "PLB", StackImplied),
 OpCode(0xAC, "LDY", Absolute),
 OpCode(0xAD, "LDA", Absolute),
 OpCode(0xAE, "LDX", Absolute),
 OpCode(0xAF, "LDA", AbsoluteLong),
 OpCode(0xB0, "BCS", ProgramCounterRelative),
 OpCode(0xB1, "LDA", DirectPageIndirectIndexedWithY),
 OpCode(0xB2, "LDA", DirectPageIndirect),
 OpCode(0xB3, "LDA", StackRelativeIndirectIndexedWithY),
 OpCode(0xB4, "LDY", DirectPageIndexedWithX),
 OpCode(0xB5, "LDA", DirectPageIndexedWithX),
 OpCode(0xB6, "LDX", DirectPageIndexedWithY),
 OpCode(0xB7, "LDA", DirectPageIndirectLongIndexedWithY),
 OpCode(0xB8, "CLV", Implied),
 OpCode(0xB9, "LDA", AbsoluteIndexedWithY),
 OpCode(0xBA, "TSX", Implied),
 OpCode(0xBB, "TYX", Implied),
 OpCode(0xBC, "LDY", AbsoluteIndexedWithX),
 OpCode(0xBD, "LDA", AbsoluteIndexedWithX),
 OpCode(0xBE, "LDX", AbsoluteIndexedWithY),
 OpCode(0xBF, "LDA", AbsoluteLongIndexedWithX),
 OpCode(0xC0, "CPY", Immediate),
 OpCode(0xC1, "CMP", DirectPageIndexedIndirectWithX),
 OpCode(0xC2, "REP", Immediate),
 OpCode(0xC3, "CMP", StackRelative),
 OpCode(0xC4, "CPY", DirectPage),
 OpCode(0xC5, "CMP", DirectPage),
 OpCode(0xC6, "DEC", DirectPage),
 OpCode(0xC7, "CMP", DirectPageIndirectLong),
 OpCode(0xC8, "INY", Implied),
 OpCode(0xC9, "CMP", Immediate),
 OpCode(0xCA, "DEX", Implied),
 OpCode(0xCB, "WAI", Implied),
 OpCode(0xCC, "CPY", Absolute),
 OpCode(0xCD, "CMP", Absolute),
 OpCode(0xCE, "DEC", Absolute),
 OpCode(0xCF, "CMP", AbsoluteLong),
 OpCode(0xD0, "BNE", ProgramCounterRelative),
 OpCode(0xD1, "CMP", DirectPageIndirectIndexedWithY),
 OpCode(0xD2, "CMP", DirectPageIndirect),
 OpCode(0xD3, "CMP", StackRelativeIndirectIndexedWithY),
 OpCode(0xD4, "PEI", StackDirectPageIndirect),
 OpCode(0xD5, "CMP", DirectPageIndexedWithX),
 OpCode(0xD6, "DEC", DirectPageIndexedWithX),
 OpCode(0xD7, "CMP", DirectPageIndirectLongIndexedWithY),
 OpCode(0xD8, "CLD", Implied),
 OpCode(0xD9, "CMP", AbsoluteIndexedWithY),
 OpCode(0xDA, "PHX", StackImplied),
 OpCode(0xDB, "STP", Implied),
 OpCode(0xDC, "JMP", AbsoluteIndirectLong),
 OpCode(0xDD, "CMP", AbsoluteIndexedWithX),
 OpCode(0xDE, "DEC", AbsoluteIndexedWithX),
 OpCode(0xDF, "CMP", AbsoluteLongIndexedWithX),
 OpCode(0xE0, "CPX", Immediate),
 OpCode(0xE1, "SBC", DirectPageIndexedIndirectWithX),
 OpCode(0xE2, "SEP", Immediate),
 OpCode(0xE3, "SBC", StackRelative),
 OpCode(0xE4, "CPX", DirectPage),
 OpCode(0xE5, "SBC", DirectPage),
 OpCode(0xE6, "INC", DirectPage),
 OpCode(0xE7, "SBC", DirectPageIndirectLong),
 OpCode(0xE8, "INX", Implied),
 OpCode(0xE9, "SBC", Immediate),
 OpCode(0xEA, "NOP", Implied),
 OpCode(0xEB, "XBA", Implied),
 OpCode(0xEC, "CPX", Absolute),
 OpCode(0xED, "SBC", Absolute),
 OpCode(0xEE, "INC", Absolute),
 OpCode(0xEF, "SBC", AbsoluteLong),
 OpCode(0xF0, "BEQ", ProgramCounterRelative),
 OpCode(0xF1, "SBC", DirectPageIndirectIndexedWithY),
 OpCode(0xF2, "SBC", DirectPageIndirect),
 OpCode(0xF3, "SBC", StackRelativeIndirectIndexedWithY),
 OpCode(0xF4, "PEA", StackAbsolute),
 OpCode(0xF5, "SBC", DirectPageIndexedWithX),
 OpCode(0xF6, "INC", DirectPageIndexedWithX),
 OpCode(0xF7, "SBC", DirectPageIndirectLongIndexedWithY),
 OpCode(0xF8, "SED", Implied),
 OpCode(0xF9, "SBC", AbsoluteIndexedWithY),
 OpCode(0xFA, "PLX", StackImplied),
 OpCode(0xFB, "XCE", Implied),
 OpCode(0xFC, "JSR", AbsoluteIndexedIndirect),
 OpCode(0xFD, "SBC", AbsoluteIndexedWithX),
 OpCode(0xFE, "INC", AbsoluteIndexedWithX),
 OpCode(0xFF, "SBC", AbsoluteLongIndexedWithX)
};

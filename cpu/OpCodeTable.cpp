
#ifndef OPCODE_TABLE_HPP
#define OPCODE_TABLE_HPP

#include "Cpu65816.hpp"

OpCode Cpu65816::OP_CODE_TABLE[] = {
    OpCode(0x00, "BRK", AddressingMode::Interrupt,                            &Cpu65816::executeInterrupt),
    OpCode(0x01, "ORA", AddressingMode::DirectPageIndexedIndirectWithX,       &Cpu65816::executeORA),
    OpCode(0x02, "COP", AddressingMode::Interrupt,                            &Cpu65816::executeInterrupt),
    OpCode(0x03, "ORA", AddressingMode::StackRelative,                        &Cpu65816::executeORA),
    OpCode(0x04, "TSB", AddressingMode::DirectPage,                           &Cpu65816::executeTSBTRB),
    OpCode(0x05, "ORA", AddressingMode::DirectPage,                           &Cpu65816::executeORA),
    OpCode(0x06, "ASL", AddressingMode::DirectPage,                           &Cpu65816::executeASL),
    OpCode(0x07, "ORA", AddressingMode::DirectPageIndirectLong,               &Cpu65816::executeORA),
    OpCode(0x08, "PHP", AddressingMode::StackImplied,                         &Cpu65816::executeStack),
    OpCode(0x09, "ORA", AddressingMode::Immediate,                            &Cpu65816::executeORA),
    OpCode(0x0A, "ASL", AddressingMode::Accumulator,                          &Cpu65816::executeASL),
    OpCode(0x0B, "PHD", AddressingMode::StackImplied,                         &Cpu65816::executeStack),
    OpCode(0x0C, "TSB", AddressingMode::Absolute,                             &Cpu65816::executeTSBTRB),
    OpCode(0x0D, "ORA", AddressingMode::Absolute,                             &Cpu65816::executeORA),
    OpCode(0x0E, "ASL", AddressingMode::Absolute,                             &Cpu65816::executeASL),
    OpCode(0x0F, "ORA", AddressingMode::AbsoluteLong,                         &Cpu65816::executeORA),
    OpCode(0x10, "BPL", AddressingMode::ProgramCounterRelative,               &Cpu65816::executeBranch),
    OpCode(0x11, "ORA", AddressingMode::DirectPageIndirectIndexedWithY,       &Cpu65816::executeORA),
    OpCode(0x12, "ORA", AddressingMode::DirectPageIndirect,                   &Cpu65816::executeORA),
    OpCode(0x13, "ORA", AddressingMode::StackRelativeIndirectIndexedWithY,    &Cpu65816::executeORA),
    OpCode(0x14, "TRB", AddressingMode::DirectPage,                           &Cpu65816::executeTSBTRB),
    OpCode(0x15, "ORA", AddressingMode::DirectPageIndexedWithX,               &Cpu65816::executeORA),
    OpCode(0x16, "ASL", AddressingMode::DirectPageIndexedWithX,               &Cpu65816::executeASL),
    OpCode(0x17, "ORA", AddressingMode::DirectPageIndirectLongIndexedWithY,   &Cpu65816::executeORA),
    OpCode(0x18, "CLC", AddressingMode::Implied,                              &Cpu65816::executeStatusReg),
    OpCode(0x19, "ORA", AddressingMode::AbsoluteIndexedWithY,                 &Cpu65816::executeORA),
    OpCode(0x1A, "INC", AddressingMode::Accumulator,                          &Cpu65816::executeINCDEC),
    OpCode(0x1B, "TCS", AddressingMode::Implied,                              &Cpu65816::executeTransfer),
    OpCode(0x1C, "TRB", AddressingMode::Absolute,                             &Cpu65816::executeTSBTRB),
    OpCode(0x1D, "ORA", AddressingMode::AbsoluteIndexedWithX,                 &Cpu65816::executeORA),
    OpCode(0x1E, "ASL", AddressingMode::AbsoluteIndexedWithX,                 &Cpu65816::executeASL),
    OpCode(0x1F, "ORA", AddressingMode::AbsoluteLongIndexedWithX,             &Cpu65816::executeORA),
    OpCode(0x20, "JSR", AddressingMode::Absolute,                             &Cpu65816::executeJumpReturn),
    OpCode(0x21, "AND", AddressingMode::DirectPageIndexedIndirectWithX,       &Cpu65816::executeAND),
    OpCode(0x22, "JSR", AddressingMode::AbsoluteLong,                         &Cpu65816::executeJumpReturn),
    OpCode(0x23, "AND", AddressingMode::StackRelative,                        &Cpu65816::executeAND),
    OpCode(0x24, "BIT", AddressingMode::DirectPage,                           &Cpu65816::executeBIT),
    OpCode(0x25, "AND", AddressingMode::DirectPage,                           &Cpu65816::executeAND),
    OpCode(0x26, "ROL", AddressingMode::DirectPage,                           &Cpu65816::executeROL),
    OpCode(0x27, "AND", AddressingMode::DirectPageIndirectLong,               &Cpu65816::executeAND),
    OpCode(0x28, "PLP", AddressingMode::StackImplied,                         &Cpu65816::executeStack),
    OpCode(0x29, "AND", AddressingMode::Immediate,                            &Cpu65816::executeAND),
    OpCode(0x2A, "ROL", AddressingMode::Accumulator,                          &Cpu65816::executeROL),
    OpCode(0x2B, "PLD", AddressingMode::StackImplied,                         &Cpu65816::executeStack),
    OpCode(0x2C, "BIT", AddressingMode::Absolute,                             &Cpu65816::executeBIT),
    OpCode(0x2D, "AND", AddressingMode::Absolute,                             &Cpu65816::executeAND),
    OpCode(0x2E, "ROL", AddressingMode::Absolute,                             &Cpu65816::executeROL),
    OpCode(0x2F, "AND", AddressingMode::AbsoluteLong,                         &Cpu65816::executeAND),
    OpCode(0x30, "BMI", AddressingMode::ProgramCounterRelative,               &Cpu65816::executeBranch),
    OpCode(0x31, "AND", AddressingMode::DirectPageIndirectIndexedWithY,       &Cpu65816::executeAND),
    OpCode(0x32, "AND", AddressingMode::DirectPageIndirect,                   &Cpu65816::executeAND),
    OpCode(0x33, "AND", AddressingMode::StackRelativeIndirectIndexedWithY,    &Cpu65816::executeAND),
    OpCode(0x34, "BIT", AddressingMode::DirectPageIndexedWithX,               &Cpu65816::executeBIT),
    OpCode(0x35, "AND", AddressingMode::DirectPageIndexedWithX,               &Cpu65816::executeAND),
    OpCode(0x36, "ROL", AddressingMode::DirectPageIndexedWithX,               &Cpu65816::executeROL),
    OpCode(0x37, "AND", AddressingMode::DirectPageIndirectLongIndexedWithY,   &Cpu65816::executeAND),
    OpCode(0x38, "SEC", AddressingMode::Implied,                              &Cpu65816::executeStatusReg),
    OpCode(0x39, "AND", AddressingMode::AbsoluteIndexedWithY,                 &Cpu65816::executeAND),
    OpCode(0x3A, "DEC", AddressingMode::Accumulator,                          &Cpu65816::executeINCDEC),
    OpCode(0x3B, "TSC", AddressingMode::Implied,                              &Cpu65816::executeTransfer),
    OpCode(0x3C, "BIT", AddressingMode::AbsoluteIndexedWithX,                 &Cpu65816::executeBIT),
    OpCode(0x3D, "AND", AddressingMode::AbsoluteIndexedWithX,                 &Cpu65816::executeAND),
    OpCode(0x3E, "ROL", AddressingMode::AbsoluteIndexedWithX,                 &Cpu65816::executeROL),
    OpCode(0x3F, "AND", AddressingMode::AbsoluteLongIndexedWithX,             &Cpu65816::executeAND),
    OpCode(0x40, "RTI", AddressingMode::StackImplied,                         &Cpu65816::executeInterrupt),
    OpCode(0x41, "EOR", AddressingMode::DirectPageIndexedIndirectWithX,       &Cpu65816::executeEOR),
    OpCode(0x42, "WDM", AddressingMode::Implied,                              &Cpu65816::executeMisc),
    OpCode(0x43, "EOR", AddressingMode::StackRelative,                        &Cpu65816::executeEOR),
    OpCode(0x44, "MVP", AddressingMode::BlockMove,                            &Cpu65816::executeMisc),
    OpCode(0x45, "EOR", AddressingMode::DirectPage,                           &Cpu65816::executeEOR),
    OpCode(0x46, "LSR", AddressingMode::DirectPage,                           &Cpu65816::executeLSR),
    OpCode(0x47, "EOR", AddressingMode::DirectPageIndirectLong,               &Cpu65816::executeEOR),
    OpCode(0x48, "PHA", AddressingMode::StackImplied,                         &Cpu65816::executeStack),
    OpCode(0x49, "EOR", AddressingMode::Immediate,                            &Cpu65816::executeEOR),
    OpCode(0x4A, "LSR", AddressingMode::Accumulator,                          &Cpu65816::executeLSR),
    OpCode(0x4B, "PHK", AddressingMode::StackImplied,                         &Cpu65816::executeStack),
    OpCode(0x4C, "JMP", AddressingMode::Absolute,                             &Cpu65816::executeJumpReturn),
    OpCode(0x4D, "EOR", AddressingMode::Absolute,                             &Cpu65816::executeEOR),
    OpCode(0x4E, "LSR", AddressingMode::Absolute,                             &Cpu65816::executeLSR),
    OpCode(0x4F, "EOR", AddressingMode::AbsoluteLong,                         &Cpu65816::executeEOR),
    OpCode(0x50, "BVC", AddressingMode::ProgramCounterRelative,               &Cpu65816::executeBranch),
    OpCode(0x51, "EOR", AddressingMode::DirectPageIndirectIndexedWithY,       &Cpu65816::executeEOR),
    OpCode(0x52, "EOR", AddressingMode::DirectPageIndirect,                   &Cpu65816::executeEOR),
    OpCode(0x53, "EOR", AddressingMode::StackRelativeIndirectIndexedWithY,    &Cpu65816::executeEOR),
    OpCode(0x54, "MVN", AddressingMode::BlockMove,                            &Cpu65816::executeMisc),
    OpCode(0x55, "EOR", AddressingMode::DirectPageIndexedWithX,               &Cpu65816::executeEOR),
    OpCode(0x56, "LSR", AddressingMode::DirectPageIndexedWithX,               &Cpu65816::executeLSR),
    OpCode(0x57, "EOR", AddressingMode::DirectPageIndirectLongIndexedWithY,   &Cpu65816::executeEOR),
    OpCode(0x58, "CLI", AddressingMode::Implied,                              &Cpu65816::executeStatusReg),
    OpCode(0x59, "EOR", AddressingMode::AbsoluteIndexedWithY,                 &Cpu65816::executeEOR),
    OpCode(0x5A, "PHY", AddressingMode::StackImplied,                         &Cpu65816::executeStack),
    OpCode(0x5B, "TCD", AddressingMode::Implied,                              &Cpu65816::executeTransfer),
    OpCode(0x5C, "JMP", AddressingMode::AbsoluteLong,                         &Cpu65816::executeJumpReturn),
    OpCode(0x5D, "EOR", AddressingMode::AbsoluteIndexedWithX,                 &Cpu65816::executeEOR),
    OpCode(0x5E, "LSR", AddressingMode::AbsoluteIndexedWithX,                 &Cpu65816::executeLSR),
    OpCode(0x5F, "EOR", AddressingMode::AbsoluteLongIndexedWithX,             &Cpu65816::executeEOR),
    OpCode(0x60, "RTS", AddressingMode::StackImplied,                         &Cpu65816::executeJumpReturn),
    OpCode(0x61, "ADC", AddressingMode::DirectPageIndexedIndirectWithX,       &Cpu65816::executeADC),
    OpCode(0x62, "PER", AddressingMode::StackProgramCounterRelativeLong,      &Cpu65816::executeStack),
    OpCode(0x63, "ADC", AddressingMode::StackRelative,                        &Cpu65816::executeADC),
    OpCode(0x64, "STZ", AddressingMode::DirectPage,                           &Cpu65816::executeSTZ),
    OpCode(0x65, "ADC", AddressingMode::DirectPage,                           &Cpu65816::executeADC),
    OpCode(0x66, "ROR", AddressingMode::DirectPage,                           &Cpu65816::executeROR),
    OpCode(0x67, "ADC", AddressingMode::DirectPageIndirectLong,               &Cpu65816::executeADC),
    OpCode(0x68, "PLA", AddressingMode::StackImplied,                         &Cpu65816::executeStack),
    OpCode(0x69, "ADC", AddressingMode::Immediate,                            &Cpu65816::executeADC),
    OpCode(0x6A, "ROR", AddressingMode::Accumulator,                          &Cpu65816::executeROR),
    OpCode(0x6B, "RTL", AddressingMode::StackImplied,                         &Cpu65816::executeJumpReturn),
    OpCode(0x6C, "JMP", AddressingMode::AbsoluteIndirect,                     &Cpu65816::executeJumpReturn),
    OpCode(0x6D, "ADC", AddressingMode::Absolute,                             &Cpu65816::executeADC),
    OpCode(0x6E, "ROR", AddressingMode::Absolute,                             &Cpu65816::executeROR),
    OpCode(0x6F, "ADC", AddressingMode::AbsoluteLong,                         &Cpu65816::executeADC),
    OpCode(0x70, "BVS", AddressingMode::ProgramCounterRelative,               &Cpu65816::executeBranch),
    OpCode(0x71, "ADC", AddressingMode::DirectPageIndirectIndexedWithY,       &Cpu65816::executeADC),
    OpCode(0x72, "ADC", AddressingMode::DirectPageIndirect,                   &Cpu65816::executeADC),
    OpCode(0x73, "ADC", AddressingMode::StackRelativeIndirectIndexedWithY,    &Cpu65816::executeADC),
    OpCode(0x74, "STZ", AddressingMode::DirectPageIndexedWithX,               &Cpu65816::executeSTZ),
    OpCode(0x75, "ADC", AddressingMode::DirectPageIndexedWithX,               &Cpu65816::executeADC),
    OpCode(0x76, "ROR", AddressingMode::DirectPageIndexedWithX,               &Cpu65816::executeROR),
    OpCode(0x77, "ADC", AddressingMode::DirectPageIndirectLongIndexedWithY,   &Cpu65816::executeADC),
    OpCode(0x78, "SEI", AddressingMode::Implied,                              &Cpu65816::executeStatusReg),
    OpCode(0x79, "ADC", AddressingMode::AbsoluteIndexedWithY,                 &Cpu65816::executeADC),
    OpCode(0x7A, "PLY", AddressingMode::StackImplied,                         &Cpu65816::executeStack),
    OpCode(0x7B, "TDC", AddressingMode::Implied,                              &Cpu65816::executeTransfer),
    OpCode(0x7C, "JMP", AddressingMode::AbsoluteIndexedIndirectWithX,         &Cpu65816::executeJumpReturn),
    OpCode(0x7D, "ADC", AddressingMode::AbsoluteIndexedWithX,                 &Cpu65816::executeADC),
    OpCode(0x7E, "ROR", AddressingMode::AbsoluteIndexedWithX,                 &Cpu65816::executeROR),
    OpCode(0x7F, "ADC", AddressingMode::AbsoluteLongIndexedWithX,             &Cpu65816::executeADC),
    OpCode(0x80, "BRA", AddressingMode::ProgramCounterRelative,               &Cpu65816::executeBranch),
    OpCode(0x81, "STA", AddressingMode::DirectPageIndexedIndirectWithX,       &Cpu65816::executeSTA),
    OpCode(0x82, "BRL", AddressingMode::ProgramCounterRelativeLong,           &Cpu65816::executeBranch),
    OpCode(0x83, "STA", AddressingMode::StackRelative,                        &Cpu65816::executeSTA),
    OpCode(0x84, "STY", AddressingMode::DirectPage,                           &Cpu65816::executeSTY),
    OpCode(0x85, "STA", AddressingMode::DirectPage,                           &Cpu65816::executeSTA),
    OpCode(0x86, "STX", AddressingMode::DirectPage,                           &Cpu65816::executeSTX),
    OpCode(0x87, "STA", AddressingMode::DirectPageIndirectLong,               &Cpu65816::executeSTA),
    OpCode(0x88, "DEY", AddressingMode::Implied,                              &Cpu65816::executeINCDEC),
    OpCode(0x89, "BIT", AddressingMode::Immediate,                            &Cpu65816::executeBIT),
    OpCode(0x8A, "TXA", AddressingMode::Implied,                              &Cpu65816::executeTransfer),
    OpCode(0x8B, "PHB", AddressingMode::StackImplied,                         &Cpu65816::executeStack),
    OpCode(0x8C, "STY", AddressingMode::Absolute,                             &Cpu65816::executeSTY),
    OpCode(0x8D, "STA", AddressingMode::Absolute,                             &Cpu65816::executeSTA),
    OpCode(0x8E, "STX", AddressingMode::Absolute,                             &Cpu65816::executeSTX),
    OpCode(0x8F, "STA", AddressingMode::AbsoluteLong,                         &Cpu65816::executeSTA),
    OpCode(0x90, "BCC", AddressingMode::ProgramCounterRelative,               &Cpu65816::executeBranch),
    OpCode(0x91, "STA", AddressingMode::DirectPageIndirectIndexedWithY,       &Cpu65816::executeSTA),
    OpCode(0x92, "STA", AddressingMode::DirectPageIndirect,                   &Cpu65816::executeSTA),
    OpCode(0x93, "STA", AddressingMode::StackRelativeIndirectIndexedWithY,    &Cpu65816::executeSTA),
    OpCode(0x94, "STY", AddressingMode::DirectPageIndexedWithX,               &Cpu65816::executeSTY),
    OpCode(0x95, "STAX", AddressingMode::DirectPageIndexedWithX),
    OpCode(0x96, "STX", AddressingMode::DirectPageIndexedWithY,               &Cpu65816::executeSTX),
    OpCode(0x97, "STA", AddressingMode::DirectPageIndirectLongIndexedWithY,   &Cpu65816::executeSTA),
    OpCode(0x98, "TYA", AddressingMode::Implied,                              &Cpu65816::executeTransfer),
    OpCode(0x99, "STA", AddressingMode::AbsoluteIndexedWithY,                 &Cpu65816::executeSTA),
    OpCode(0x9A, "TXS", AddressingMode::Implied,                              &Cpu65816::executeTransfer),
    OpCode(0x9B, "TXY", AddressingMode::Implied,                              &Cpu65816::executeTransfer),
    OpCode(0x9C, "STZ", AddressingMode::Absolute,                             &Cpu65816::executeSTZ),
    OpCode(0x9D, "STA", AddressingMode::AbsoluteIndexedWithX,                 &Cpu65816::executeSTA),
    OpCode(0x9E, "STZ", AddressingMode::AbsoluteIndexedWithX,                 &Cpu65816::executeSTZ),
    OpCode(0x9F, "STA", AddressingMode::AbsoluteLongIndexedWithX,             &Cpu65816::executeSTA),
    OpCode(0xA0, "LDY", AddressingMode::Immediate,                            &Cpu65816::executeLDY),
    OpCode(0xA1, "LDA", AddressingMode::DirectPageIndexedIndirectWithX,       &Cpu65816::executeLDA),
    OpCode(0xA2, "LDX", AddressingMode::Immediate,                            &Cpu65816::executeLDX),
    OpCode(0xA3, "LDA", AddressingMode::StackRelative,                        &Cpu65816::executeLDA),
    OpCode(0xA4, "LDY", AddressingMode::DirectPage,                           &Cpu65816::executeLDY),
    OpCode(0xA5, "LDA", AddressingMode::DirectPage,                           &Cpu65816::executeLDA),
    OpCode(0xA6, "LDX", AddressingMode::DirectPage,                           &Cpu65816::executeLDX),
    OpCode(0xA7, "LDA", AddressingMode::DirectPageIndirectLong,               &Cpu65816::executeLDA),
    OpCode(0xA8, "TAY", AddressingMode::Implied,                              &Cpu65816::executeTransfer),
    OpCode(0xA9, "LDA", AddressingMode::Immediate,                            &Cpu65816::executeLDA),
    OpCode(0xAA, "TAX", AddressingMode::Implied,                              &Cpu65816::executeTransfer),
    OpCode(0xAB, "PLB", AddressingMode::StackImplied,                         &Cpu65816::executeStack),
    OpCode(0xAC, "LDY", AddressingMode::Absolute,                             &Cpu65816::executeLDY),
    OpCode(0xAD, "LDA", AddressingMode::Absolute,                             &Cpu65816::executeLDA),
    OpCode(0xAE, "LDX", AddressingMode::Absolute,                             &Cpu65816::executeLDX),
    OpCode(0xAF, "LDA", AddressingMode::AbsoluteLong,                         &Cpu65816::executeLDA),
    OpCode(0xB0, "BCS", AddressingMode::ProgramCounterRelative,               &Cpu65816::executeBranch),
    OpCode(0xB1, "LDA", AddressingMode::DirectPageIndirectIndexedWithY,       &Cpu65816::executeLDA),
    OpCode(0xB2, "LDA", AddressingMode::DirectPageIndirect,                   &Cpu65816::executeLDA),
    OpCode(0xB3, "LDA", AddressingMode::StackRelativeIndirectIndexedWithY,    &Cpu65816::executeLDA),
    OpCode(0xB4, "LDY", AddressingMode::DirectPageIndexedWithX,               &Cpu65816::executeLDY),
    OpCode(0xB5, "LDA", AddressingMode::DirectPageIndexedWithX,               &Cpu65816::executeLDA),
    OpCode(0xB6, "LDX", AddressingMode::DirectPageIndexedWithY,               &Cpu65816::executeLDX),
    OpCode(0xB7, "LDA", AddressingMode::DirectPageIndirectLongIndexedWithY,   &Cpu65816::executeLDA),
    OpCode(0xB8, "CLV", AddressingMode::Implied,                              &Cpu65816::executeStatusReg),
    OpCode(0xB9, "LDA", AddressingMode::AbsoluteIndexedWithY,                 &Cpu65816::executeLDA),
    OpCode(0xBA, "TSX", AddressingMode::Implied,                              &Cpu65816::executeTransfer),
    OpCode(0xBB, "TYX", AddressingMode::Implied,                              &Cpu65816::executeTransfer),
    OpCode(0xBC, "LDY", AddressingMode::AbsoluteIndexedWithX,                 &Cpu65816::executeLDY),
    OpCode(0xBD, "LDA", AddressingMode::AbsoluteIndexedWithX,                 &Cpu65816::executeLDA),
    OpCode(0xBE, "LDX", AddressingMode::AbsoluteIndexedWithY,                 &Cpu65816::executeLDX),
    OpCode(0xBF, "LDA", AddressingMode::AbsoluteLongIndexedWithX,             &Cpu65816::executeLDA),
    OpCode(0xC0, "CPY", AddressingMode::Immediate,                            &Cpu65816::executeCPXCPY),
    OpCode(0xC1, "CMP", AddressingMode::DirectPageIndexedIndirectWithX,       &Cpu65816::executeCMP),
    OpCode(0xC2, "REP", AddressingMode::Immediate,                            &Cpu65816::executeStatusReg),
    OpCode(0xC3, "CMP", AddressingMode::StackRelative,                        &Cpu65816::executeCMP),
    OpCode(0xC4, "CPY", AddressingMode::DirectPage,                           &Cpu65816::executeCPXCPY),
    OpCode(0xC5, "CMP", AddressingMode::DirectPage,                           &Cpu65816::executeCMP),
    OpCode(0xC6, "DEC", AddressingMode::DirectPage,                           &Cpu65816::executeINCDEC),
    OpCode(0xC7, "CMP", AddressingMode::DirectPageIndirectLong,               &Cpu65816::executeCMP),
    OpCode(0xC8, "INY", AddressingMode::Implied,                              &Cpu65816::executeINCDEC),
    OpCode(0xC9, "CMP", AddressingMode::Immediate,                            &Cpu65816::executeCMP),
    OpCode(0xCA, "DEX", AddressingMode::Implied,                              &Cpu65816::executeINCDEC),
    OpCode(0xCB, "WAI", AddressingMode::Implied),
    OpCode(0xCC, "CPY", AddressingMode::Absolute,                             &Cpu65816::executeCPXCPY),
    OpCode(0xCD, "CMP", AddressingMode::Absolute,                             &Cpu65816::executeCMP),
    OpCode(0xCE, "DEC", AddressingMode::Absolute,                             &Cpu65816::executeINCDEC),
    OpCode(0xCF, "CMP", AddressingMode::AbsoluteLong,                         &Cpu65816::executeCMP),
    OpCode(0xD0, "BNE", AddressingMode::ProgramCounterRelative,               &Cpu65816::executeBranch),
    OpCode(0xD1, "CMP", AddressingMode::DirectPageIndirectIndexedWithY,       &Cpu65816::executeCMP),
    OpCode(0xD2, "CMP", AddressingMode::DirectPageIndirect,                   &Cpu65816::executeCMP),
    OpCode(0xD3, "CMP", AddressingMode::StackRelativeIndirectIndexedWithY,    &Cpu65816::executeCMP),
    OpCode(0xD4, "PEI", AddressingMode::StackDirectPageIndirect,              &Cpu65816::executeStack),
    OpCode(0xD5, "CMP", AddressingMode::DirectPageIndexedWithX,               &Cpu65816::executeCMP),
    OpCode(0xD6, "DEC", AddressingMode::DirectPageIndexedWithX,               &Cpu65816::executeINCDEC),
    OpCode(0xD7, "CMP", AddressingMode::DirectPageIndirectLongIndexedWithY,   &Cpu65816::executeCMP),
    OpCode(0xD8, "CLD", AddressingMode::Implied,                              &Cpu65816::executeStatusReg),
    OpCode(0xD9, "CMP", AddressingMode::AbsoluteIndexedWithY,                 &Cpu65816::executeCMP),
    OpCode(0xDA, "PHX", AddressingMode::StackImplied,                         &Cpu65816::executeStack),
    OpCode(0xDB, "STP", AddressingMode::Implied),
    OpCode(0xDC, "JMP", AddressingMode::AbsoluteIndirectLong,                 &Cpu65816::executeJumpReturn),
    OpCode(0xDD, "CMP", AddressingMode::AbsoluteIndexedWithX,                 &Cpu65816::executeCMP),
    OpCode(0xDE, "DEC", AddressingMode::AbsoluteIndexedWithX,                 &Cpu65816::executeINCDEC),
    OpCode(0xDF, "CMP", AddressingMode::AbsoluteLongIndexedWithX,             &Cpu65816::executeCMP),
    OpCode(0xE0, "CPX", AddressingMode::Immediate,                            &Cpu65816::executeCPXCPY),
    OpCode(0xE1, "SBC", AddressingMode::DirectPageIndexedIndirectWithX),
    OpCode(0xE2, "SEP", AddressingMode::Immediate,                            &Cpu65816::executeStatusReg),
    OpCode(0xE3, "SBC", AddressingMode::StackRelative),
    OpCode(0xE4, "CPX", AddressingMode::DirectPage,                           &Cpu65816::executeCPXCPY),
    OpCode(0xE5, "SBC", AddressingMode::DirectPage),
    OpCode(0xE6, "INC", AddressingMode::DirectPage,                           &Cpu65816::executeINCDEC),
    OpCode(0xE7, "SBC", AddressingMode::DirectPageIndirectLong),
    OpCode(0xE8, "INX", AddressingMode::Implied,                              &Cpu65816::executeINCDEC),
    OpCode(0xE9, "SBC", AddressingMode::Immediate),
    OpCode(0xEA, "NOP", AddressingMode::Implied),
    OpCode(0xEB, "XBA", AddressingMode::Implied,                              &Cpu65816::executeMisc),
    OpCode(0xEC, "CPX", AddressingMode::Absolute,                             &Cpu65816::executeCPXCPY),
    OpCode(0xED, "SBC", AddressingMode::Absolute),
    OpCode(0xEE, "INC", AddressingMode::Absolute,                             &Cpu65816::executeINCDEC),
    OpCode(0xEF, "SBC", AddressingMode::AbsoluteLong),
    OpCode(0xF0, "BEQ", AddressingMode::ProgramCounterRelative,               &Cpu65816::executeBranch),
    OpCode(0xF1, "SBC", AddressingMode::DirectPageIndirectIndexedWithY),
    OpCode(0xF2, "SBC", AddressingMode::DirectPageIndirect),
    OpCode(0xF3, "SBC", AddressingMode::StackRelativeIndirectIndexedWithY),
    OpCode(0xF4, "PEA", AddressingMode::StackAbsolute,                        &Cpu65816::executeStack),
    OpCode(0xF5, "SBC", AddressingMode::DirectPageIndexedWithX),
    OpCode(0xF6, "INC", AddressingMode::DirectPageIndexedWithX,               &Cpu65816::executeINCDEC),
    OpCode(0xF7, "SBC", AddressingMode::DirectPageIndirectLongIndexedWithY),
    OpCode(0xF8, "SED", AddressingMode::Implied,                              &Cpu65816::executeStatusReg),
    OpCode(0xF9, "SBC", AddressingMode::AbsoluteIndexedWithY),
    OpCode(0xFA, "PLX", AddressingMode::StackImplied,                         &Cpu65816::executeStack),
    OpCode(0xFB, "XCE", AddressingMode::Implied,                              &Cpu65816::executeStatusReg),
    OpCode(0xFC, "JSR", AddressingMode::AbsoluteIndexedIndirectWithX,         &Cpu65816::executeJumpReturn),
    OpCode(0xFD, "SBC", AddressingMode::AbsoluteIndexedWithX),
    OpCode(0xFE, "INC", AddressingMode::AbsoluteIndexedWithX,                 &Cpu65816::executeINCDEC),
    OpCode(0xFF, "SBC", AddressingMode::AbsoluteLongIndexedWithX)
};

#endif // OPCODE_TABLE_HPP


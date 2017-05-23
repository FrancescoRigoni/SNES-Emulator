#ifndef ADDRESSING_HPP
#define ADDRESSING_HPP

enum AddressingMode {
    Interrupt,
    Accumulator,
    BlockMove,
    Implied,
    Immediate,
    Absolute,
    AbsoluteProgram,         // Absolute using Program Bank instead of Data Bank
    AbsoluteLong,
    AbsoluteIndirect,
    AbsoluteIndirectLong,
    AbsoluteIndexedIndirect, // index with X
    AbsoluteIndexedWithX,
    AbsoluteLongIndexedWithX,
    AbsoluteIndexedWithY,
    DirectPage,
    DirectPageIndexedWithX,
    DirectPageIndexedWithY,
    DirectPageIndirect,
    DirectPageIndirectLong,
    DirectPageIndexedIndirectWithX,
    DirectPageIndirectIndexedWithY,
    DirectPageIndirectLongIndexedWithY,
    StackImplied,
    StackRelative,
    StackAbsolute,
    StackDirectPageIndirect,
    StackProgramCounterRelativeLong,
    StackRelativeIndirectIndexedWithY,
    ProgramCounterRelative,
    ProgramCounterRelativeLong
};

#endif // ADDRESSING_HPP

#ifndef ADDRESSING_HPP
#define ADDRESSING_HPP

enum class AddressingMode {
    Interrupt,
    Accumulator,
    BlockMove,
    Implied,
    Immediate,
    Absolute,
    AbsoluteLong,
    AbsoluteIndirect,
    AbsoluteIndirectLong,
    AbsoluteIndexedIndirectWithX, // index with X
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

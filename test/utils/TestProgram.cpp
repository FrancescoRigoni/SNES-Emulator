
#include "TestProgram.hpp"

TestProgram FUP::accumulatorTo16Bit() {
    TestProgram p;
    p.addInstruction(0xC2).withOperand8(0x20);          // REP #%00100000
    return p;
}

TestProgram FUP::accumulatorTo8Bit() {
    TestProgram p;
    p.addInstruction(0xE2).withOperand8(0x20);          // SEP #%00100000
    return p;
}

TestProgram FUP::indexTo16Bit() {
    TestProgram p;
    p.addInstruction(0xC2).withOperand8(0x10);          // REP #%00010000
    return p;
}

TestProgram FUP::indexTo8Bit() {
    TestProgram p;
    p.addInstruction(0xE2).withOperand8(0x10);          // SEP #%00010000
    return p;
}

TestProgram FUP::load16BitValueIntoAccumulator(uint16_t value) {
    TestProgram p;
    p.addProgram(accumulatorTo16Bit());
    p.addInstruction(0xA9).withOperand16(value);        // LDA Immediate
    return p;
}

TestProgram FUP::load8BitValueIntoAccumulator(uint8_t value) {
    TestProgram p;
    p.addProgram(accumulatorTo8Bit());
    p.addInstruction(0xA9).withOperand8(value);         // LDA Immediate
    return p;
}

TestProgram FUP::load16BitValueIntoIndexX(uint16_t value) {
    TestProgram p;
    p.addProgram(indexTo16Bit());
    // Save the A register
    p.addInstruction(0x48);                             // PHA
    // Save status
    p.addInstruction(0x08);                             // PHP
    p.addProgram(accumulatorTo16Bit());
    p.addInstruction(0xA9).withOperand16(value);        // LDA Immediate
    p.addInstruction(0xAA);                             // TAX
    // Restore status
    p.addInstruction(0x28);                             // PLP
    // Restore the A register
    p.addInstruction(0x68);                             // PLA
    return p;
}

TestProgram FUP::load8BitValueIntoIndexX(uint8_t value) {
    TestProgram p;
    p.addProgram(indexTo8Bit());
    // Save the A register
    p.addInstruction(0x48);                             // PHA
    // Save status
    p.addInstruction(0x08);                             // PHP
    p.addProgram(accumulatorTo8Bit());
    p.addInstruction(0xA9).withOperand8(value);         // LDA Immediate
    p.addInstruction(0xAA);                             // TAX
    // Restore status
    p.addInstruction(0x28);                             // PLP
    // Restore the A register
    p.addInstruction(0x68);                             // PLA
    return p;
}

TestProgram FUP::load16BitValueIntoIndexY(uint16_t value) {
    TestProgram p;
    p.addProgram(indexTo16Bit());
    // Save the A register
    p.addInstruction(0x48);                             // PHA
    // Save status
    p.addInstruction(0x08);                             // PHP
    p.addProgram(accumulatorTo16Bit());
    p.addInstruction(0xA9).withOperand16(value);        // LDA Immediate
    p.addInstruction(0xA8);                             // TAY
    // Restore status
    p.addInstruction(0x28);                             // PLP
    // Restore the A register
    p.addInstruction(0x68);                             // PLA
    return p;
}

TestProgram FUP::load8BitValueIntoIndexY(uint8_t value) {
    TestProgram p;
    p.addProgram(indexTo8Bit());
    // Save the A register
    p.addInstruction(0x48);                             // PHA
    // Save status
    p.addInstruction(0x08);                             // PHP
    p.addProgram(accumulatorTo8Bit());
    p.addInstruction(0xA9).withOperand8(value);         // LDA Immediate
    p.addInstruction(0xA8);                             // TAY
    // Restore status
    p.addInstruction(0x28);                             // PLP
    // Restore the A register
    p.addInstruction(0x68);                             // PLA
    return p;
}

TestProgram FUP::switchCpuToNativeMode() {
    TestProgram p;
    p.addInstruction(0x18);                             // CLC
    p.addInstruction(0xFB);                             // XCE
    return p;
}

TestProgram FUP::setDataBankTo(uint8_t value) {
    TestProgram p;
    // Save the A register
    p.addInstruction(0x48);                             // PHA
    // Save status
    p.addInstruction(0x08);                             // PHP
    // Make sure accumulator is 8 bit
    p.addInstruction(0xE2).withOperand8(0x20);          // SEP #%00100000
    p.addInstruction(0xA9).withOperand8(value);         // LDA Immediate
    p.addInstruction(0x48);                             // PHA (Push Accumulator)
    p.addInstruction(0xAB);                             // PLB (Pull Data Bank)
    // Restore status
    p.addInstruction(0x28);                             // PLP
    // Restore the A register
    p.addInstruction(0x68);                             // PLA
    return p;
}

TestProgram FUP::setDirectPageTo(uint16_t value) {
    TestProgram p;
    // Save the A register
    p.addInstruction(0x48);                             // PHA
    // Save status
    p.addInstruction(0x08);                             // PHP
    // Make sure accumulator is 16 bit
    p.addProgram(accumulatorTo16Bit());
    p.addInstruction(0xA9).withOperand16(value);        // LDA Immediate
    p.addInstruction(0x48);                             // PHA (Push Accumulator)
    p.addInstruction(0x2B);                             // PLD (Pull Direct Page)
    // Restore status
    p.addInstruction(0x28);                             // PLP
    // Restore the A register
    p.addInstruction(0x68);                             // PLA
    return p;
}

TestProgram FUP::store16BitAtOffsetInDirectPage(uint8_t offset, uint16_t value) {
    TestProgram p;
    // Save the A register
    p.addInstruction(0x48);                             // PHA
    // Save status
    p.addInstruction(0x08);                             // PHP
    // Make sure accumulator is 16 bit
    p.addProgram(accumulatorTo16Bit());
    p.addInstruction(0xA9).withOperand16(value);        // LDA Absolute
    p.addInstruction(0x85).withOperand8(offset);        // STA Direct Page
    // Restore status
    p.addInstruction(0x28);                             // PLP
    // Restore the A register
    p.addInstruction(0x68);                             // PLA
    return p;
}
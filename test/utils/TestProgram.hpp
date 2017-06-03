
#ifndef SNESEMU_TESTPROGRAM_H
#define SNESEMU_TESTPROGRAM_H

#include <stdint.h>
#include <vector>
#include <Device.hpp>

class TestProgram {
    private:
        std::vector<uint8_t> mData;
        int mInstructionsCount = 0;

    public:
        TestProgram &addInstruction(uint8_t opCode) {
            mData.push_back(opCode);
            ++mInstructionsCount;
            return *this;
        }

        TestProgram &withOperand8(uint8_t operand) {
            mData.push_back(operand);
            return *this;
        }

        TestProgram &withOperand16(uint16_t operand) {
            uint8_t lowByte = operand & 0xFF;
            uint8_t highByte = operand >> 8;
            mData.push_back(lowByte);
            mData.push_back(highByte);
            return *this;
        }

        TestProgram &withOperandAddr(const Address &address) {
            uint8_t lowByte = address.getOffset() & 0xFF;
            uint8_t highByte = address.getOffset() >> 8;
            mData.push_back(lowByte);
            mData.push_back(highByte);
            mData.push_back(address.getBank());
            return *this;
        }

        TestProgram &addProgram(const TestProgram &program) {
            mData.insert(std::end(mData), std::begin(program.mData), std::end(program.mData));
            mInstructionsCount += program.mInstructionsCount;
            return *this;
        }

        int getInstructionsCount() const {
            return mInstructionsCount;
        }

        void copyProgramBytes(uint8_t *buffer) const {
            for (auto byte : mData) {
                *buffer = byte;
                buffer++;
            }
        }
};

// Frequently Used Programs
class FUP {
    public:
        static TestProgram accumulatorTo16Bit();
        static TestProgram accumulatorTo8Bit();
        static TestProgram indexTo16Bit();
        static TestProgram indexTo8Bit();
        static TestProgram load16BitValueIntoAccumulator(uint16_t);
        static TestProgram load8BitValueIntoAccumulator(uint8_t);
        static TestProgram load16BitValueIntoIndexX(uint16_t);
        static TestProgram load8BitValueIntoIndexX(uint8_t);
        static TestProgram load16BitValueIntoIndexY(uint16_t);
        static TestProgram load8BitValueIntoIndexY(uint8_t);
        static TestProgram switchCpuToNativeMode();
        static TestProgram setDataBankTo(uint8_t);
        static TestProgram setDirectPageTo(uint16_t);
        static TestProgram store16BitAtOffsetInDirectPage(uint8_t, uint16_t);
};

#endif //SNESEMU_TESTPROGRAM_H

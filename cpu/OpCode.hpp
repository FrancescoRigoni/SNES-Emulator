#ifndef OPCODE_HPP
#define OPCODE_HPP

#include <stdint.h>

#include "Addressing.hpp"

class Cpu;

class OpCode {
    private:
        uint8_t mCode;
        const char * const mName;
        AddressingMode mAddressingMode;
        void (Cpu::*mExecutor)(OpCode &);

    public:
        OpCode(uint8_t code, const char * const name, const AddressingMode &addressingMode) :
            mCode(code), mName(name), mAddressingMode(addressingMode), mExecutor(0) {
        }

        OpCode(uint8_t code, const char * const name, const AddressingMode &addressingMode, void (Cpu::*executor)(OpCode &)) :
            mCode(code), mName(name), mAddressingMode(addressingMode), mExecutor(executor) {
        }

        const uint8_t getCode() {
            return mCode;
        }

        const char *getName() {
            return mName;
        }

        const AddressingMode getAddressingMode() {
            return mAddressingMode;
        }

        const bool execute(Cpu &cpu) {
            if (mExecutor != 0) {
                OpCode opCode = *this;
                (cpu.*mExecutor)(opCode);
                return true;
            }
            return false;
        }
};

#endif // OPCODE_HPP

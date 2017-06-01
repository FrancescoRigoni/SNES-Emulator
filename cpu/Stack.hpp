
#ifndef __STACK__
#define __STACK__

#include <stdint.h>

#include  "MemoryMapper.hpp"

#define STACK_POINTER_DEFAULT 0x1FF

class Stack {
    public:

        Stack(MemoryMapper *);
        Stack(MemoryMapper *, uint16_t);
        
        void push8Bit(uint8_t);
        void push16Bit(uint16_t);
        
        uint8_t pull8Bit();
        uint16_t pull16Bit();
        
        uint16_t getStackPointer();
        
    private:
        MemoryMapper *mMemoryMapper;
        Address mStackAddress;
};

#endif

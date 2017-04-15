
#include "Stack.hpp"
#include "Log.hpp"

#define LOG_TAG "Stack"

Stack::Stack(MemoryMapper *memoryMapper) :
    mMemoryMapper(memoryMapper) {
    mStackAddress.bank = 0;
    mStackAddress.offset = STACK_POINTER_DEFAULT;
    Log::trc(LOG_TAG).str("Initialized at default location ").sp().hex(mStackAddress.offset, 4).show();
}

Stack::Stack(MemoryMapper *memoryMapper, uint16_t stackPointer) :
    mMemoryMapper(memoryMapper) {
    mStackAddress.bank = 0;
    mStackAddress.offset = stackPointer;
    Log::trc(LOG_TAG).str("Initialized at location ").sp().hex(mStackAddress.offset, 4).show();    
}
        
void Stack::push8Bit(uint8_t value) {
    mMemoryMapper->storeByte(mStackAddress, value);
    mStackAddress.offset--;
}

void Stack::push16Bit(uint16_t value) {
    uint8_t leastSignificant = (uint8_t)((value) & 0xFF);
    uint8_t mostSignificant =  (uint8_t)(((value) & 0xFF00) >> 8);
    push8Bit(mostSignificant);
    push8Bit(leastSignificant);
}
        
uint8_t Stack::pull8Bit() {
    ++mStackAddress.offset;
    return mMemoryMapper->readByte(mStackAddress);
}

uint16_t Stack::pull16Bit() {
    return (uint16_t)(pull8Bit() | (((uint16_t)pull8Bit()) << 8));
}

uint16_t Stack::getStackPointer() {
    return mStackAddress.offset;
}

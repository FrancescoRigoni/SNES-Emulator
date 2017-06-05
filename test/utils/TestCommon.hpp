
#ifndef SNESEMU_TESTCOMMON
#define SNESEMU_TESTCOMMON

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Test_STA
#include <boost/test/unit_test.hpp>

#include "Interrupt.hpp"
#include "Cpu65816.hpp"

#include "MockRam.hpp"
#include "TestProgram.hpp"

/**
 * This header file defines a common infrastructure used by all tests.
 * */

#define MOCK_RAM_SIZE               0x40000
#define RESET_INTERRUPT_ADDRESS      0x2000

MemoryMapper *memoryMapper;
MockRam *mockRam;
Cpu65816 *cpu;

EmulationModeInterrupts emulationInterrupts {
    .coProcessorEnable = 0x00,
    .unused = 0x00,
    .abort = 0x00,
    .nonMaskableInterrupt = 0x00,
    .reset = RESET_INTERRUPT_ADDRESS,
    .brkIrq = 0x00,
};

NativeModeInterrupts nativeInterrupts {
    .coProcessorEnable = 0x00,
    .brk = 0x00,
    .abort = 0x00,
    .nonMaskableInterrupt = 0x00,
    .reset = 0x00,
    .interruptRequest = 0x00,
};

struct SuiteFixture {
    SuiteFixture() {
        memoryMapper = new MemoryMapper();
        mockRam = new MockRam(*memoryMapper, MOCK_RAM_SIZE);
        cpu = new Cpu65816(*memoryMapper, &emulationInterrupts, &nativeInterrupts);
        cpu->setRESPin(false);
    }
    ~SuiteFixture() {
        delete cpu;
        delete mockRam;
        delete memoryMapper;
    }
};

#define INIT_TEST_PROGRAM() \
    TestProgram p

#define LOAD_AND_RUN_TEST_PROGRAM() \
    mockRam->loadProgram(p, Address(0x00, RESET_INTERRUPT_ADDRESS)); \
    for (auto i = 0; i < p.getInstructionsCount(); i++) cpu->executeNextInstruction()

#endif //SNESEMU_TESTCOMMON

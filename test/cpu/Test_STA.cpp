#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Test_STA
#include <boost/test/unit_test.hpp>

#include "MockRam.hpp"
#include "Interrupt.hpp"
#include "Cpu65816.hpp"

#define MOCK_RAM_SIZE               0x4000
#define RESET_INTERRUPT_ADDRESS     0x2000

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
    }
    ~SuiteFixture() {
        delete cpu;
        delete mockRam;
        delete memoryMapper;
    }
};

BOOST_FIXTURE_TEST_SUITE(SUITE_LDA_STA, SuiteFixture)

    BOOST_AUTO_TEST_CASE(STA_ABSOLUTE_16)
    {
        const int instructionsCount = 7;
        uint8_t instructions[] = {
            0xA9, 0x00,                  // LDA #$00
            0x48,                        // PHA (Push Accumulator)
            0xAB,                        // PLB (Pull Data Bank)
            0x18,                        // CLC
            0xFB,                        // XCE
            0xA9, 0x01, 0xC0,            // LDA #$C001
            0x8D, 0x00, 0x10,            // STA $1000
        };
        mockRam->copyDataBlock(instructions, RESET_INTERRUPT_ADDRESS, sizeof(instructions));

        for (int i = 0; i < instructionsCount; i++) cpu->executeNext();

        BOOST_CHECK(memoryMapper->readTwoBytes(Address(0x00, 0x1000)) == 0xC001);
    }

    BOOST_AUTO_TEST_CASE(STA_ABSOLUTE_LONG_16)
    {
        const int instructionsCount = 4;
        uint8_t instructions[] = {
            0x18,                        // CLC
            0xFB,                        // XCE
            0xA9, 0x01, 0xC0,            // LDA #$C001
            0x8F, 0x00, 0x00, 0x01,      // STA $010000
        };
        mockRam->copyDataBlock(instructions, RESET_INTERRUPT_ADDRESS, sizeof(instructions));

        for (int i = 0; i < instructionsCount; i++) cpu->executeNext();

        BOOST_CHECK(memoryMapper->readTwoBytes(Address(0x01, 0x0000)) == 0xC001);
    }

BOOST_AUTO_TEST_SUITE_END()

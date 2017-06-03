
#include "TestCommon.hpp"

BOOST_FIXTURE_TEST_SUITE(SUITE_TEST_STA, SuiteFixture)

    BOOST_AUTO_TEST_CASE(STA_ABSOLUTE_A_8)
    {
        INIT_TEST_PROGRAM();

        Address storeAddress(0x01, 0x9000);
        uint8_t storedValue = 0xA5;

        p.addProgram(FUP::setDataBankTo(storeAddress.getBank()));
        p.addProgram(FUP::load8BitValueIntoAccumulator(storedValue));

        p.addInstruction(0x8D).withOperand16(storeAddress.getOffset());     // STA Absolute

        LOAD_AND_RUN_TEST_PROGRAM();

        BOOST_CHECK(memoryMapper->readByte(storeAddress) == storedValue);
    }

    BOOST_AUTO_TEST_CASE(STA_ABSOLUTE_A_16)
    {
        INIT_TEST_PROGRAM();

        Address storeAddress(0x03, 0x9000);
        uint16_t storedValue = 0xC12F;

        p.addProgram(FUP::setDataBankTo(storeAddress.getBank()));
        p.addProgram(FUP::switchCpuToNativeMode());
        p.addProgram(FUP::load16BitValueIntoAccumulator(storedValue));

        p.addInstruction(0x8D).withOperand16(storeAddress.getOffset());     // STA Absolute

        LOAD_AND_RUN_TEST_PROGRAM();

        BOOST_CHECK(memoryMapper->readTwoBytes(storeAddress) == storedValue);
    }

    BOOST_AUTO_TEST_CASE(STA_ABSOLUTE_LONG_A_16)
    {
        INIT_TEST_PROGRAM();

        Address storeAddress(0x03, 0x9000);
        uint16_t storedValue = 0xC12F;

        p.addProgram(FUP::switchCpuToNativeMode());
        p.addProgram(FUP::load16BitValueIntoAccumulator(storedValue));

        p.addInstruction(0x8F).withOperandAddr(storeAddress);               // STA Absolute Long

        LOAD_AND_RUN_TEST_PROGRAM();

        BOOST_CHECK(memoryMapper->readTwoBytes(storeAddress) == storedValue);
    }

    BOOST_AUTO_TEST_CASE(STA_ZERO_PAGE_A_8)
    {
        INIT_TEST_PROGRAM();

        // Zero page is always in bank zero
        Address storeAddress(0x00, 0x90);
        uint8_t storedValue = 0x87;

        p.addProgram(FUP::load8BitValueIntoAccumulator(storedValue));
        p.addInstruction(0x85).withOperand8(storeAddress.getOffset());      // STA Direct Page

        LOAD_AND_RUN_TEST_PROGRAM();

        BOOST_CHECK(memoryMapper->readByte(storeAddress) == storedValue);
    }

    BOOST_AUTO_TEST_CASE(STA_DIRECT_PAGE_A_16)
    {
        INIT_TEST_PROGRAM();

        uint16_t directPageValue = 0x3456;
        uint8_t offsetInDirectPage = 0x17;
        Address storeAddress(0x00, directPageValue + offsetInDirectPage);
        uint16_t storedValue = 0xAB87;

        p.addProgram(FUP::switchCpuToNativeMode());
        p.addProgram(FUP::setDirectPageTo(directPageValue));
        p.addProgram(FUP::load16BitValueIntoAccumulator(storedValue));

        p.addInstruction(0x85).withOperand8(offsetInDirectPage);    // STA Direct Page

        LOAD_AND_RUN_TEST_PROGRAM();

        BOOST_CHECK(memoryMapper->readTwoBytes(storeAddress) == storedValue);
    }

    BOOST_AUTO_TEST_CASE(STA_DIRECT_PAGE_INDIRECT_A_16)
    {
        INIT_TEST_PROGRAM();

        uint16_t directPageValue = 0x3000;
        uint8_t indirectAddressOffsetInDirectPage = 0x45;

        Address storeAddress(0x02, 0x1234);
        uint16_t storedValue = 0x97FC;

        p.addProgram(FUP::setDataBankTo(storeAddress.getBank()));
        p.addProgram(FUP::switchCpuToNativeMode());
        p.addProgram(FUP::setDirectPageTo(directPageValue));
        p.addProgram(FUP::store16BitAtOffsetInDirectPage(indirectAddressOffsetInDirectPage, storeAddress.getOffset()));
        p.addProgram(FUP::load16BitValueIntoAccumulator(storedValue));

        p.addInstruction(0x92).withOperand8(indirectAddressOffsetInDirectPage);     // STA Direct Page Indirect

        LOAD_AND_RUN_TEST_PROGRAM();

        BOOST_CHECK(memoryMapper->readTwoBytes(storeAddress) == storedValue);
    }

//    BOOST_AUTO_TEST_CASE(STA_ZERO_PAGE_INDIRECT_8)
//    {
//        /**
//         * Cpu in emulation mode
//         * Set Data Bank register to the desired value
//         * Cpu in native mode
//         * Setup indirect address in direct page
//         * Use STA (Direct Page Indirect) to store the value using direct page indirect
//         * Check that the correct value is in memory at the desired offset in direct page
//         */
//        INIT_TEST_PROGRAM();
//
//        uint16_t directPageValue = 0x3000;
//        uint8_t indirectAddressOffsetInDirectPage = 0x45;
//
//        Address storeAddress(0x02, 0x1234);
//        uint16_t storedValue = 0x97FC;
//
//        p.addProgram(FUP::setDataBankTo(storeAddress.getBank()));
//        p.addProgram(FUP::switchCpuToNativeMode());
//        p.addProgram(FUP::setDirectPageTo(directPageValue));
//        p.addProgram(FUP::store16BitAtOffsetInDirectPage(indirectAddressOffsetInDirectPage, storeAddress.getOffset()));
//
//        // Load in the A register the value to be stored and store it
//        p.addInstruction(0xA9).withOperand16(storedValue);                          // LDA Absolute
//        p.addInstruction(0x92).withOperand8(indirectAddressOffsetInDirectPage);     // STA Direct Page Indirect
//
//        LOAD_AND_RUN_TEST_PROGRAM();
//
//        BOOST_CHECK(memoryMapper->readTwoBytes(storeAddress) == storedValue);
//    }

BOOST_AUTO_TEST_SUITE_END()


#include "TestCommon.hpp"

BOOST_FIXTURE_TEST_SUITE(SUITE_TEST_STX, SuiteFixture)

    BOOST_AUTO_TEST_CASE(STX_ABSOLUTE_I_8_A_8)
    {
        INIT_TEST_PROGRAM();

        Address storeAddress(0x00, 0x9000);
        uint8_t storedValue = 0xA5;

        p.addProgram(FUP::setDataBankTo(storeAddress.getBank()));
        p.addProgram(FUP::move8BitValueToXRegister(storedValue));

        p.addInstruction(0x8E).withOperand16(storeAddress.getOffset());     // STX Absolute

        LOAD_AND_RUN_TEST_PROGRAM();

        BOOST_CHECK(memoryMapper->readByte(storeAddress) == storedValue);
    }

    BOOST_AUTO_TEST_CASE(STX_ABSOLUTE_I_16_A_16)
    {
        INIT_TEST_PROGRAM();

        Address storeAddress(0x00, 0x9000);
        uint16_t storedValue = 0x34A5;

        p.addProgram(FUP::switchCpuToNativeMode());
        p.addProgram(FUP::setDataBankTo(storeAddress.getBank()));
        p.addProgram(FUP::move16BitValueToXRegister(storedValue));

        p.addInstruction(0x8E).withOperand16(storeAddress.getOffset());     // STX Absolute

        LOAD_AND_RUN_TEST_PROGRAM();

        BOOST_CHECK(memoryMapper->readTwoBytes(storeAddress) == storedValue);
    }

//    BOOST_AUTO_TEST_CASE(STX_DIRECT_PAGE_16)
//    {
//        /**
//         * Cpu in native mode
//         * Set Direct Page register to the desired offset
//         * Load a value in the A register
//         * Use STX (Direct Page) to store A to the desired offset in direct page
//         * Check that the correct value is in memory at the desired offset in direct page
//         */
//        INIT_TEST_PROGRAM();
//
//        uint16_t directPageValue = 0x3456;
//        uint8_t offsetInDirectPage = 0x17;
//        Address storeAddress(0x00, directPageValue + offsetInDirectPage);
//        uint16_t storedValue = 0xAB87;
//
//        p.addProgram(FUP::switchCpuToNativeMode());
//        p.addProgram(FUP::setDirectPageTo(directPageValue));
//        p.addInstruction(0xA9).withOperand16(storedValue);          // LDA Absolute
//        p.addInstruction(0x85).withOperand8(offsetInDirectPage);    // STX Direct Page
//
//        LOAD_AND_RUN_TEST_PROGRAM();
//
//        BOOST_CHECK(memoryMapper->readTwoBytes(storeAddress) == storedValue);
//    }

BOOST_AUTO_TEST_SUITE_END()


#include "TestCommon.hpp"

BOOST_FIXTURE_TEST_SUITE(SUITE_TEST_STY, SuiteFixture)

    BOOST_AUTO_TEST_CASE(STY_ABSOLUTE_I_8)
    {
        INIT_TEST_PROGRAM();

        Address storeAddress(0x00, 0x9000);
        uint8_t storedValue = 0xA5;

        p.addProgram(FUP::setDataBankTo(storeAddress.getBank()));
        p.addProgram(FUP::load8BitValueIntoIndexY(storedValue));

        p.addInstruction(0x8C).withOperand16(storeAddress.getOffset());     // STY Absolute

        LOAD_AND_RUN_TEST_PROGRAM();

        BOOST_CHECK(memoryMapper->readByte(storeAddress) == storedValue);
    }

    BOOST_AUTO_TEST_CASE(STY_ABSOLUTE_I_16)
    {
        INIT_TEST_PROGRAM();

        Address storeAddress(0x00, 0x9000);
        uint16_t storedValue = 0x34A5;

        p.addProgram(FUP::switchCpuToNativeMode());
        p.addProgram(FUP::setDataBankTo(storeAddress.getBank()));
        p.addProgram(FUP::load16BitValueIntoIndexY(storedValue));

        p.addInstruction(0x8C).withOperand16(storeAddress.getOffset());     // STY Absolute

        LOAD_AND_RUN_TEST_PROGRAM();

        BOOST_CHECK(memoryMapper->readTwoBytes(storeAddress) == storedValue);
    }

    BOOST_AUTO_TEST_CASE(STY_DIRECT_PAGE_I_8)
    {
        INIT_TEST_PROGRAM();

        uint16_t directPage = 0x0;
        uint8_t offsetInDirectPage = 0x17;
        Address storeEffectiveAddress(0x00, directPage + offsetInDirectPage);
        uint16_t storedValue = 0xAB;

        p.addProgram(FUP::load8BitValueIntoIndexY(storedValue));

        p.addInstruction(0x84).withOperand8(offsetInDirectPage);    // STY Direct Page

        LOAD_AND_RUN_TEST_PROGRAM();

        BOOST_CHECK(memoryMapper->readTwoBytes(storeEffectiveAddress) == storedValue);
    }

    BOOST_AUTO_TEST_CASE(STY_DIRECT_PAGE_I_16)
    {
        INIT_TEST_PROGRAM();

        uint16_t directPage = 0x3456;
        uint8_t offsetInDirectPage = 0x17;
        Address storeEffectiveAddress(0x00, directPage + offsetInDirectPage);
        uint16_t storedValue = 0xAB87;

        p.addProgram(FUP::switchCpuToNativeMode());
        p.addProgram(FUP::setDirectPageTo(directPage));
        p.addProgram(FUP::load16BitValueIntoIndexY(storedValue));

        p.addInstruction(0x84).withOperand8(offsetInDirectPage);    // STY Direct Page

        LOAD_AND_RUN_TEST_PROGRAM();

        BOOST_CHECK(memoryMapper->readTwoBytes(storeEffectiveAddress) == storedValue);
    }

    BOOST_AUTO_TEST_CASE(STY_DIRECT_PAGE_INDEXED_Y_I_8)
    {
        INIT_TEST_PROGRAM();

        uint8_t indexRegisterYValue = 0x34;
        uint8_t offsetInZeroPage = 0x17;
        Address storeEffectiveAddress(0x00, offsetInZeroPage + indexRegisterYValue);
        uint8_t storedValue = 0x21;

        p.addProgram(FUP::load8BitValueIntoIndexX(indexRegisterYValue));
        p.addProgram(FUP::load8BitValueIntoIndexY(storedValue));

        p.addInstruction(0x94).withOperand8(offsetInZeroPage);    // STY Direct Page Indexed, X

        LOAD_AND_RUN_TEST_PROGRAM();

        BOOST_CHECK(memoryMapper->readByte(storeEffectiveAddress) == storedValue);
    }

    BOOST_AUTO_TEST_CASE(STY_DIRECT_PAGE_INDEXED_Y_I_16)
    {
        INIT_TEST_PROGRAM();

        uint16_t directPage = 0x3256;
        uint16_t indexRegisterYValue = 0x1111;
        uint8_t offsetInDirectPage = 0x67;
        Address storeEffectiveAddress(0x00, directPage + offsetInDirectPage + indexRegisterYValue);
        uint16_t storedValue = 0x92AB;

        p.addProgram(FUP::switchCpuToNativeMode());
        p.addProgram(FUP::setDirectPageTo(directPage));
        p.addProgram(FUP::load16BitValueIntoIndexX(indexRegisterYValue));
        p.addProgram(FUP::load16BitValueIntoIndexY(storedValue));

        p.addInstruction(0x94).withOperand8(offsetInDirectPage);    // STY Direct Page Indexed, X

        LOAD_AND_RUN_TEST_PROGRAM();

        BOOST_CHECK(memoryMapper->readTwoBytes(storeEffectiveAddress) == storedValue);
    }

BOOST_AUTO_TEST_SUITE_END()

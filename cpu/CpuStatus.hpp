
#include <stdint.h>

class CpuStatus {
    public:
        CpuStatus();
    
        void setZeroFlag();
        void clearZeroFlag();
        bool zeroFlag();
        
        void setSignFlag();
        void clearSignFlag();
        bool signFlag();
        
        void setDecimalFlag();
        void clearDecimalFlag();
        bool decimalFlag();
        
        void setInterruptDisableFlag();
        void clearInterruptDisableFlag();
        bool interruptDisableFlag();
        
        void setAccumulatorWidthFlag();
        void clearAccumulatorWidthFlag();
        bool accumulatorWidthFlag();

        void setIndexWidthFlag();
        void clearIndexWidthFlag();
        bool indexWidthFlag();
        
        void setCarryFlag();
        void clearCarryFlag();
        bool carryFlag();
        
        void setBreakFlag();
        void clearBreakFlag();
        bool breakFlag();
        
        void setOverflowFlag();
        void clearOverflowFlag();
        bool overflowFlag();
        
        void setEmulationFlag();
        void clearEmulationFlag();
        bool emulationFlag();
        
        uint8_t getRegisterValue();
        void setRegisterValue(uint8_t);
        
        void updateZeroFlagFrom8BitValue(uint8_t);
        void updateZeroFlagFrom16BitValue(uint16_t);
        void updateSignFlagFrom8BitValue(uint8_t);
        void updateSignFlagFrom16BitValue(uint16_t);
        void updateSignAndZeroFlagFrom8BitValue(uint8_t);
        void updateSignAndZeroFlagFrom16BitValue(uint16_t);
    
    private:
        bool mZeroFlag = false;
        bool mSignFlag = false;
        bool mDecimalFlag = false;
        bool mInterruptDisableFlag = false;
        bool mAccumulatorWidthFlag = false;
        bool mIndexWidthFlag = false;
        bool mCarryFlag = false;
        bool mEmulationFlag = false;
        bool mOverflowFlag = false;
        bool mBreakFlag = false;
};

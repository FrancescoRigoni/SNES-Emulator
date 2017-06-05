
#ifndef SNESEMU_CPU65816DEBUGGER_H
#define SNESEMU_CPU65816DEBUGGER_H

#include <cstdint>
#include <functional>

#include <Device.hpp>
#include "BuildConfig.hpp"
#include "Cpu65816.hpp"

class Cpu65816Debugger {
    public:
        Cpu65816Debugger(Cpu65816 &);

        void step();
        void setBreakPoint(const Address &);
        void dumpCpu() const ;
        void logStatusRegister() const ;
        void logOpCode(OpCode &) const ;

        void doBeforeStep(std::function<void ()>);
        void doAfterStep(std::function<void ()>);
        void onBreakPoint(std::function<void ()>);

    private:
        std::function<void ()> mOnBeforeStepHandler;
        std::function<void ()> mOnAfterStepHandler;
        std::function<void ()> mOnBreakPointHandler;

        // Let's assume $00:$0000 is not a valid address for code
        Address mBreakPointAddress {0x00, 0x0000};
        bool mBreakpointHit = false;

        Cpu65816 &mCpu;
};

#endif //SNESEMU_CPU65816INTRUMENTATION_H

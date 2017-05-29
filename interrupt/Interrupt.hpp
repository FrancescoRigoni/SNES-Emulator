#ifndef INTERRUPT_HPP
#define INTERRUPT_HPP

// Interrupt table. Native mode.
typedef struct {
    const uint16_t coProcessorEnable;
    const uint16_t brk;
    const uint16_t abort;
    const uint16_t nonMaskableInterrupt;
    const uint16_t reset;
    const uint16_t interruptRequest;
} NativeModeInterrupts;

// Interrupt table. Emulation mode.
typedef struct {
    const uint16_t coProcessorEnable;
    const uint16_t unused;
    const uint16_t abort;
    const uint16_t nonMaskableInterrupt;
    const uint16_t reset;
    const uint16_t brkIrq;
} EmulationModeInterrupts;

#endif // INTERRUPT_HPP

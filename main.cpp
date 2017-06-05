
#include "Cpu65816.hpp"
#include "Cartridge.hpp"
#include "Rom.hpp"
#include "MemoryMapper.hpp"
#include "Ram.hpp"
#include "Log.hpp"

#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#define LOG_TAG "MAIN"

namespace InterruptTable {
    NativeModeInterrupts *native;
    EmulationModeInterrupts *emulation;
}

static struct termios oldt;

void restore_terminal_settings(void)
{
    tcsetattr(0, TCSANOW, &oldt);  /* Apply saved settings */
}

void disable_waiting_for_enter(void)
{
    struct termios newt;

    /* Make terminal read 1 char at a time */
    tcgetattr(0, &oldt);  /* Save terminal settings */
    newt = oldt;  /* Init new settings */
    newt.c_lflag &= ~(ICANON | ECHO);  /* Change settings */
    tcsetattr(0, TCSANOW, &newt);  /* Apply settings */
}

int main(int argc, char **argv) {
	Log::vrb(LOG_TAG).str("+++ SNES EMULATOR WANNABE +++").show();

	disable_waiting_for_enter();

	// Initialize memory mapper
	MemoryMapper memoryMapper = MemoryMapper();

	// Initialize cartridge
    Cartridge cartridgeReader = Cartridge(std::string(argv[1]), memoryMapper);

    if (cartridgeReader.getRomType() == RomType::LO_ROM) {
        InterruptTable::native = (NativeModeInterrupts *) (cartridgeReader.getRomData() + NATIVE_INTERRUPT_TABLE_LOROM);
        InterruptTable::emulation = (EmulationModeInterrupts *) cartridgeReader.getRomData() + EMULATED_INTERRUPT_TABLE_LOROM;
    } else {
        InterruptTable::native = (NativeModeInterrupts *) (cartridgeReader.getRomData() + NATIVE_INTERRUPT_TABLE_HIROM);
        InterruptTable::emulation = (EmulationModeInterrupts *) (cartridgeReader.getRomData() + EMULATED_INTERRUPT_TABLE_HIROM);
    }

    // Initialize RAM
    Ram ram(memoryMapper);

    // Initialize cpu
    Cpu65816 cpu(memoryMapper, InterruptTable::emulation, InterruptTable::native);

    //cpu.setBreakPoint(0x00, 0x8023);

	//char i;
	/*while (cpu.executeNext()){
		//usleep(200*1000);
	};*/

    // Set RES pin low to enable CPU to run
    cpu.setRESPin(false);

	while (true){
		/*char c = getchar();
		if (c == 'z') cpu.debug_setZeroFlag();
		else if (c == 'd') cpu.logCpuStatus();
		else if (c == 'q') break;
		else */cpu.executeNext();
       // usleep(50*1000);
	};

	restore_terminal_settings();

}

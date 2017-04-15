
#include "Cpu.hpp"
#include "RomReader.hpp"
#include "MemoryMapper.hpp"
#include "Log.hpp"

#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#define LOG_TAG "MAIN"

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
	
    RomReader reader = RomReader(std::string(argv[1]));
    MemoryMapper memoryMapper = MemoryMapper(reader);
    Cpu cpu(reader, memoryMapper);
    
    cpu.setBreakPoint(0x00, 0x8023);

	char i;
	/*while (cpu.executeNext()){
		//usleep(200*1000);
	};*/
	
	while (true){
		char c = getchar();
		if (c == 'z') cpu.debug_setZeroFlag();
		else if (c == 'd') cpu.logCpuStatus();
		else if (c == 'q') break;
		else cpu.executeNext();
	};
	
	restore_terminal_settings();

}

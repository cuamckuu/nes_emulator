#include <iostream>
#include <cstdint>

#include "include/rom.h"
#include "include/cpu.h"

using byte = uint8_t;
using word = uint16_t;

int main() {
	ROM rom = load_rom("nestest.nes");

	CPU cpu;
	cpu.load_rom_to_memory(rom);

	cpu.init();
	cpu.reset();
	cpu.PC = 0xC000;

	cpu.run(415, true);

	return 0;
}

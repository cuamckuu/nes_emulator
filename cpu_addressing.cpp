#include "include/cpu.h"

void CPU::cpu_address_immediate() {
    op_value = memory[PC++];
};

void CPU::cpu_address_zero_page() {
    op_address = memory[PC++] & 0xFF;
    op_value = memory[op_address];
};

void CPU::cpu_address_absolute() {
    op_address = memory[PC] | (memory[PC+1] << 8);
    op_value = memory[op_address];
    PC += 2;
};

void CPU::cpu_address_absolute_x() {
    byte op = memory[PC - 1];
    op_address = (memory[PC] | (memory[PC+1] << 8)) + X;
    op_value = memory[op_address];
    PC += 2;

    // Page crossed
    if ((op_address & 0xFF00) != ((op_address - X) & 0xFF00)
            && op != 0xDE && op != 0xDD
            && op != 0xFE && op != 0x5E
            && op != 0x3E && op != 0x7E
            && op != 0x9d) {
		extra_cycles++;
    }
};

void CPU::cpu_address_absolute_y() {
    op_address = (memory[PC] | (memory[PC+1] << 8)) + Y;
    op_value = memory[op_address];
    PC += 2;

    // Page crossed
    if ((op_address & 0xFF00) != ((op_address - Y) & 0xFF00)) {
		extra_cycles++;
    }
};

void CPU::cpu_address_implied() {};

void CPU::cpu_address_zero_page_x() {
    op_address = (memory[PC++] + X) & 0xFF;
    op_value = memory[op_address];
};

void CPU::cpu_address_zero_page_y() {
    op_address = (memory[PC++] + Y) & 0xFF;
    op_value = memory[op_address];
};

void CPU::cpu_address_indirect() {
	// Strange thing from LittleNES

    word arg_addr = (memory[PC] | (memory[PC+1] << 8));

    // The famous 6502 bug when instead of reading from $C0FF/$C100 it reads from $C0FF/$C000
    if ((arg_addr & 0xFF) == 0xFF) {
        // Buggy code
        op_address = (memory[arg_addr & 0xFF00] << 8) + memory[arg_addr];
    } else {
        // Normal code
    	op_address = (memory[arg_addr] | (memory[arg_addr+1] << 8));
    }
    PC += 2;
};

void CPU::cpu_address_indirect_x() {
    byte arg_addr = (memory[PC++] + X);

    if (arg_addr == 0xFF) {
        op_address = (memory[0x00] << 8) | memory[0xff];
    } else {
        op_address = (memory[(arg_addr + 1)] << 8) | memory[arg_addr % 0xff];
    }
    op_value = memory[op_address];
};

void CPU::cpu_address_indirect_y() {
    byte arg_addr = memory[PC++];

    if (arg_addr == 0xFF) {
        op_address = ((memory[0x00] << 8) | memory[0xff]) + Y;
    } else {
        op_address = ((memory[arg_addr + 1] << 8) | memory[arg_addr]) + Y;
    }
    op_value = memory[op_address];

    // Page crossed
    // FIXME: Magic
    if ((op_address & 0xFF00) != ((op_address - Y) & 0xFF00)) {
		extra_cycles++;
    }
};

void CPU::cpu_address_relative() {
    op_address = memory[PC++];

	if (op_address & 0x80) {
        op_address -= 0x100;
	}

    op_address += PC;

    // FIXME: Don't know where to add this extra cycle
    if ((op_address >> 8) != (PC >> 8)) {
		//extra_cycles++;
    }
};

#include "include/cpu.h"

void CPU::cpu_address_immediate() {
    op_value = memory[PC++];
};

void CPU::cpu_address_zero_page() {
    op_address = memory[PC++];
    op_value = memory[op_address];
};

void CPU::cpu_address_absolute() {
    op_address = memory[PC] | (memory[PC+1] << 8);
    op_value = memory[op_address];
    PC += 2;
};

void CPU::cpu_address_absolute_x() {
    op_address = (memory[PC] | (memory[PC+1] << 8)) + X;
    op_value = memory[op_address];
    PC += 2;

    // Page crossed
    if ((op_address >> 8) != (PC >> 8)) {
		extra_cycles++;
    }
};

void CPU::cpu_address_absolute_y() {
    op_address = (memory[PC] | (memory[PC+1] << 8)) + Y;
    op_value = memory[op_address];
    PC += 2;

    // Page crossed
    if ((op_address >> 8) != (PC >> 8)) {
		extra_cycles++;
    }
};

void CPU::cpu_address_implied() {};

void CPU::cpu_address_zero_page_x() {
    op_address = memory[PC++] + X;
    op_value = memory[op_address];
};

void CPU::cpu_address_zero_page_y() {
    op_address = memory[PC++] + Y;
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
    byte arg_addr = memory[PC++];
    op_address = (memory[arg_addr + X + 1] << 8) | memory[arg_addr + X];
    op_value = memory[op_address];
};

void CPU::cpu_address_indirect_y() {
    byte arg_addr = memory[PC++];
    op_address = ((memory[arg_addr + 1] << 8) | memory[arg_addr]) + Y;
    op_value = memory[op_address];

};

void CPU::cpu_address_relative() {
    op_address = memory[PC++];

	if (op_address & 0x80) {
        op_address -= 0x100;
	}

    op_address += PC;

    if ((op_address >> 8) != (PC >> 8)) {
		extra_cycles++;
    }
};

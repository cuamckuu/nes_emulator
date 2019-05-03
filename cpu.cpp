#include "include/cpu_instructions.h"
#include "include/cpu.h"
#include "include/rom.h"

inline bool is_power_of_2(byte n) {
    return ((n & (n-1)) == 0 && (n > 0));
}

bool CPU::is_flag_set(byte flag) {
    // Works only for one flag per call
    return is_power_of_2(P & flag);
}

void CPU::load_rom_to_memory(ROM rom) {
    byte mmc_id = (rom.header.flags7 & 0xF0) | ((rom.header.flags6 & 0xF0) >> 4);
    std::cout << "Mapper ID: " << std::hex << (int)mmc_id << "\n";

    if (mmc_id != 0) {
        std::cout << "Error: Only mapper 0 games are supported\n";
        exit(1);
    }

    if (rom.header.prg_size == 1) {
        std::memcpy(&memory[0x8000], rom.prg_data, 0x4000);
        std::memcpy(&memory[0xC000], rom.prg_data, 0x4000);
    } else if (rom.header.prg_size == 2) {
        std::memcpy(&memory[0x8000], rom.prg_data, 0x8000);
    } else {
        std::cout << "Error: Wrong PRG_SIZE for mapper 0\n";
        exit(1);
    }

    std::cout << "ROM loaded to memory\n\n";
}

void CPU::init() {
    P = 0x24;
    SP = 0;
    A = X = Y = 0;
}

void CPU::reset() {
    PC = (memory[0xFFFD] << 8) + memory[0xFFFC];

    SP -= 3;
    P |= interrupt_flag;
}

void CPU::run(long cycles, bool debug=false) {
    while (cycles > 0) {
        byte op_code = memory[PC++];
        Instruction inst = get_instruction(op_code);
        if (op_code == 0xff || inst.name[0] == 'N') {
            inst.log_info();
            std::cout << "\nInstruction not implemented.\n";
            break;
        }

        if (debug) {
            std::cout << std::setw(4) << std::hex << (int)(PC-1) << " ";
            inst.log_info();
            log_state();
        }

        (this->*inst.address_func_ptr)();
        (this->*inst.func_ptr)();

        inst.cycles += extra_cycles;
        cycles -= inst.cycles;
        extra_cycles = 0;

        cpu_cycles += inst.cycles;

        if (cpu_cycles == 0 || cpu_cycles == 0 || cpu_cycles == 0) {
            log_stack();
        }
    }
}

void CPU::log_state() {
    std::cout << "A:"    << std::setw(2) << std::hex << (int)A;
    std::cout << " X:"   << std::setw(2) << std::hex << (int)X;
    std::cout << " Y:"   << std::setw(2) << std::hex << (int)Y;
    std::cout << " P:"   << std::setw(2) << std::hex << (int)P;
    std::cout << " SP:"  << std::setw(2) << std::hex << (int)SP;
    std::cout << " CYC:" << std::setw(5) << std::dec << cpu_cycles;
    std::cout << std::endl;
}

void CPU::log_stack() {
    for (int i = 0x0100; i <= 0x01FF; i++) {
        int val = memory[i];
        if (val != 0) {
            std::cout << std::hex << i << ":" << val << std::endl;
        }
    }
}

void CPU::update_carry_flag(bool is_set) {
    if (is_set) {
        P |= carry_flag;
    } else {
        P &= ~carry_flag;
    }
};

void CPU::update_zero_flag(word value) {
    if (value == 0) {
        P |= zero_flag;
    } else {
        P &= ~zero_flag;
    }
};

void CPU::update_interrupt_flag(word value) {
    std::cout << "INTERRUPT FLAG NOT IMPLEMENTED\n";
};

void CPU::update_decimal_flag(word value) {
    std::cout << "DEDIMAL FLAG NOT IMPLEMENTED\n";
};

void CPU::update_break_flag(word value) {
    std::cout << "BREAK FLAG NOT IMPLEMENTED\n";
};

void CPU::update_overflow_flag(bool is_set) {
    if (is_set) {
        P |= overflow_flag;
    } else {
        P &= ~overflow_flag;
    }
};

void CPU::update_negative_flag(word value) {
    if ((value >> 7) & 0x1) {
        P |= negative_flag;
    } else {
        P &= ~negative_flag;
    }
};

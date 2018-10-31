#ifndef CPU_INSTRUCTIONS_H
#define CPU_INSTRUCTIONS_H

#include <cstdint>

struct CPU;

using byte = uint8_t;
using word = uint16_t;

enum CPU_P_FLAG {
    carry_flag     = 0x01,
    zero_flag      = 0x02,
    interrupt_flag = 0x04,
    decimal_flag   = 0x08,
    break_flag     = 0x10,
    unused_flag    = 0x20,
    overflow_flag  = 0x40,
    negative_flag  = 0x80
};

struct Instruction {
    // Add default unknown function

    byte op_code = 0xff;
    char name[4] = "NUL";
    byte bytes = 0x00;
    byte cycles = 0x01; // To escape loops

    void (CPU::* func_ptr)();
    void (CPU::* address_func_ptr)();

    void log_info();
};


#endif //CPU_INSTRUCTIONS_H

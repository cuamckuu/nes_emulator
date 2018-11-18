/*
 *  This file is based on the following reference: http://obelisk.me.uk/6502/reference.html
 *
 */

#include "include/cpu_instructions.h"
#include "include/cpu.h"

void Instruction::log_info() {
    std::cout << std::left;
    std::cout << " OP: 0x"   << std::setw(10) << std::hex << (int)op_code;
    std::cout << " NAME: "   << std::setw(5)  << name;
    std::cout << " BYTES: "  << std::setw(5)  << (int)bytes;
    std::cout << " CYCLES: " << std::setw(5)  << (int)cycles;
    std::cout << std::endl;
}

#define INST(_opc, _name, _bytes, _cycles, _mode) \
    if (_opc == op_code) { \
        memcpy(&result.name, #_name, 4); \
        result.bytes = _bytes; \
        result.cycles = _cycles; \
        result.func_ptr = &cpu_##_name; \
        result.address_func_ptr = &cpu_address_##_mode; \
    } \

Instruction CPU::get_instruction(byte op_code) {
    Instruction result;
    result.op_code = op_code; // To see unimplemented OP code
    result.func_ptr = &cpu_nul; // Prevent segfault if instruction not implemented
    result.address_func_ptr = &cpu_address_implied;

    INST(0xff, nul, 1, 1, implied);
    INST(0x78, sei, 1, 2, implied);
    INST(0xd8, cld, 1, 2, implied);
    INST(0x9a, txs, 1, 2, implied);
    INST(0xad, lda, 3, 4, absolute);
    INST(0x10, bpl, 2, 2, relative);
    INST(0x86, stx, 2, 3, zero_page);
    INST(0x20, jsr, 3, 6, absolute);
    INST(0xea, nop, 1, 2, implied);
    INST(0x38, sec, 1, 2, implied);
    INST(0xb0, bcs, 2, 2, relative);
    INST(0x18, clc, 1, 2, implied);
    INST(0x58, cli, 1, 2, implied);
    INST(0xb8, clv, 1, 2, implied);
    INST(0x90, bcc, 2, 2, relative);
    INST(0xf0, beq, 2, 2, relative);
    INST(0xd0, bne, 2, 2, relative);
    INST(0x85, sta, 2, 3, zero_page);
    INST(0x24, bit, 2, 3, zero_page);
    INST(0x70, bvs, 2, 2, relative);
    INST(0x50, bvc, 2, 2, relative);
    INST(0x60, rts, 1, 6, implied);
    INST(0x30, bmi, 2, 2, relative);
    INST(0xf8, sed, 1, 2, implied);

    INST(0xa0, ldy, 2, 2, immediate);
    INST(0xa4, ldy, 2, 3, zero_page);
    INST(0xb4, ldy, 2, 4, zero_page_x);
    INST(0xac, ldy, 3, 4, absolute);
    INST(0xbc, ldy, 3, 4, absolute_x);

    INST(0xa2, ldx, 2, 2, immediate);
    INST(0xa6, ldx, 2, 3, zero_page);
    INST(0xb6, ldx, 2, 4, zero_page_x);
    INST(0xae, ldx, 3, 4, absolute);
    INST(0xbe, ldx, 3, 4, absolute_x);

    INST(0xa9, lda, 2, 2, immediate);
    INST(0xa5, lda, 2, 3, zero_page);
    INST(0xb5, lda, 2, 4, zero_page_x);
    INST(0xad, lda, 3, 4, absolute);
    INST(0xbd, lda, 3, 4, absolute_x);
    INST(0xb9, lda, 3, 4, absolute_y);
    INST(0xa1, lda, 2, 6, indirect_x);
    INST(0xb1, lda, 2, 5, indirect_y);

    INST(0x08, php, 1, 3, implied);
    INST(0x68, pla, 1, 4, implied);

    INST(0x29, and, 2, 2, immediate);
    INST(0x25, and, 2, 3, zero_page);
    INST(0x35, and, 2, 4, zero_page_x);
    INST(0x2d, and, 3, 4, absolute);
    INST(0x3d, and, 3, 4, absolute_x);
    INST(0x39, and, 3, 4, absolute_y);
    INST(0x21, and, 2, 6, indirect_x);
    INST(0x31, and, 2, 5, indirect_y);

    INST(0xc9, cmp, 2, 2, immediate);
    INST(0xc5, cmp, 2, 3, zero_page);
    INST(0xd5, cmp, 2, 4, zero_page_x);
    INST(0xcd, cmp, 3, 4, absolute);
    INST(0xdd, cmp, 3, 4, absolute_x);
    INST(0xd9, cmp, 3, 4, absolute_y);
    INST(0xc1, cmp, 2, 6, indirect_x);
    INST(0xd1, cmp, 2, 5, indirect_y);

    INST(0x48, pha, 1, 3, implied);
    INST(0x28, plp, 1, 4, implied);

    INST(0x4c, jmp, 3, 3, absolute);
    INST(0x6c, jmp, 3, 5, indirect);

    INST(0xaa, tax, 1, 2, implied);
    INST(0xa8, tay, 1, 2, implied);
    INST(0x8a, txa, 1, 2, implied);
    INST(0x98, tya, 1, 2, implied);

    INST(0x09, ora, 2, 2, immediate);
    INST(0x05, ora, 2, 3, zero_page);
    INST(0x15, ora, 2, 4, zero_page_x);
    INST(0x0d, ora, 3, 4, absolute);
    INST(0x1d, ora, 3, 4, absolute_x);
    INST(0x19, ora, 3, 4, absolute_y);
    INST(0x01, ora, 2, 6, indirect_x);
    INST(0x11, ora, 2, 5, indirect_y);

    INST(0x49, eor, 2, 2, immediate);
    INST(0x45, eor, 2, 3, zero_page);
    INST(0x45, eor, 2, 4, zero_page_x);
    INST(0x4d, eor, 3, 4, absolute);
    INST(0x5d, eor, 3, 4, absolute_x);
    INST(0x59, eor, 3, 4, absolute_y);
    INST(0x41, eor, 2, 6, indirect_x);
    INST(0x51, eor, 2, 5, indirect_y);

    INST(0xe8, inx, 1, 2, implied);
    INST(0xc8, iny, 1, 2, implied);

    return result;
}

#undef INST

void CPU::cpu_nul() {};

//Storage
void CPU::cpu_lda() {
// - load a with m
    A = op_value;

    if (A == 0) {
        P |= zero_flag;
    } else {
        P &= ~zero_flag;
    }

    if ((A >> 7) & 0x1) {
        P |= negative_flag;
    } else {
        P &= ~negative_flag;
    }
};

void CPU::cpu_ldx() {
// - load x with m
    X = op_value;
    if (X == 0) {
        P |= zero_flag;
    } else {
        P &= ~zero_flag;
    }

    if ((X >> 7) & 0x1) {
        P |= negative_flag;
    } else {
        P &= ~negative_flag;
    }
};

void CPU::cpu_ldy() {
// - load y with m
    Y = op_value;
    if (Y == 0) {
        P |= zero_flag;
    } else {
        P &= ~zero_flag;
    }

    if ((Y >> 7) & 0x1) {
        P |= negative_flag;
    } else {
        P &= ~negative_flag;
    }
};

void CPU::cpu_sta() {
// - store a in m
    memory[op_address] = A;
};

void CPU::cpu_stx() {
// - store x in m
    memory[op_address] = X;
};

void CPU::cpu_sty() {
// - store y in m

};

void CPU::cpu_tax() {
// - transfer a to x
    X = A;

    if (X == 0) {
        P |= zero_flag;
    } else {
        P &= ~zero_flag;
    }

    if ((X >> 7) & 0x01) {
        P |= negative_flag;
    } else {
        P &= ~negative_flag;
    }
};

void CPU::cpu_tay() {
// - transfer a to y
    Y = A;

    if (Y == 0) {
        P |= zero_flag;
    } else {
        P &= ~zero_flag;
    }

    if ((Y >> 7) & 0x01) {
        P |= negative_flag;
    } else {
        P &= ~negative_flag;
    }
};

void CPU::cpu_tsx() {
// - transfer stack pointer to x
    X = SP;
};

void CPU::cpu_txa() {
// - transfer x to a
    A = X;

    if (A == 0) {
        P |= zero_flag;
    } else {
        P &= ~zero_flag;
    }

    if ((A >> 7) & 0x01) {
        P |= negative_flag;
    } else {
        P &= ~negative_flag;
    }
};

void CPU::cpu_txs() {
// - transfer x to stack pointer
    SP = X;
};

void CPU::cpu_tya() {
// - transfer y to a
    A = Y;

    if (A == 0) {
        P |= zero_flag;
    } else {
        P &= ~zero_flag;
    }

    if ((A >> 7) & 0x01) {
        P |= negative_flag;
    } else {
        P &= ~negative_flag;
    }
};

//Math
void CPU::cpu_adc() {
// - add m to a with carry

};

void CPU::cpu_dec() {
// - decrement m by one

};

void CPU::cpu_dex() {
// - decrement x by one

};

void CPU::cpu_dey() {
// - decrement y by one

};

void CPU::cpu_inc() {
// - increment m by one

};

void CPU::cpu_inx() {
// - increment x by one
    X += 1;

    if (X == 0) {
        P |= zero_flag;
    } else {
        P &= ~zero_flag;
    }

    if ((X >> 7) & 0x01) {
        P |= negative_flag;
    } else {
        P &= ~negative_flag;
    }
};

void CPU::cpu_iny() {
// - increment y by one
    Y += 1;

    if (Y == 0) {
        P |= zero_flag;
    } else {
        P &= ~zero_flag;
    }

    if ((Y >> 7) & 0x01) {
        P |= negative_flag;
    } else {
        P &= ~negative_flag;
    }
};

void CPU::cpu_sbc() {
// - subtract m from a with borrow

};

//Bitwise
void CPU::cpu_and() {
// - "and" m with a
    A &= op_value;

    if (A == 0) {
        P |= zero_flag;
    } else {
        P &= ~zero_flag;
    }

    if ((A >> 7) & 0x1) {
        P |= negative_flag;
    } else {
        P &= ~negative_flag;
    }
};

void CPU::cpu_asl() {
// - shift left one bit (m or a)

};

void CPU::cpu_bit() {
// - test bits in m with a
    if ((A & op_value) == 0) {
        P |= zero_flag;
    } else {
        P &= ~zero_flag;
    }

    if ((op_value >> 6) & 0x1) {
        P |= overflow_flag;
    } else {
        P &= ~overflow_flag;
    }

    if ((op_value >> 7) & 0x1) {
        P |= negative_flag;
    } else {
        P &= ~negative_flag;
    }
};

void CPU::cpu_eor() {
// - "exclusive-or" m with a
    A ^= op_value;

    if (A == 0) {
        P |= zero_flag;
    } else {
        P &= ~zero_flag;
    }

    if ((A >> 7) & 0x1) {
        P |= negative_flag;
    } else {
        P &= ~negative_flag;
    }
};

void CPU::cpu_lsr() {
// - shift right one bit (m or a)

};

void CPU::cpu_ora() {
// - "or" m with a
    A |= op_value;

    if (A == 0) {
        P |= zero_flag;
    } else {
        P &= ~zero_flag;
    }

    if ((A >> 7) & 0x1) {
        P |= negative_flag;
    } else {
        P &= ~negative_flag;
    }
};

void CPU::cpu_rol() {
// - rotate one bit left (m or a)

};

void CPU::cpu_ror() {
// - rotate one bit right (m or a)

};

//Branch
void CPU::cpu_bcc() {
// - branch on carry clear
    if (!is_flag_set(carry_flag)) {
        // Page crossed
        if ((op_address >> 8) != (PC >> 8)) {
            extra_cycles += 2;
        }

        PC = op_address;
        extra_cycles++;
    }
};

void CPU::cpu_bcs() {
// - branch on carry set
    if (is_flag_set(carry_flag)) {
        // Page crossed
        if ((op_address >> 8) != (PC >> 8)) {
            extra_cycles += 2;
        }

        PC = op_address;
        extra_cycles++;
    }
};

void CPU::cpu_beq() {
// - branch on result zero
    if (is_flag_set(zero_flag)) {
        // Page crossed
        if ((op_address >> 8) != (PC >> 8)) {
            extra_cycles += 2;
        }

        PC = op_address;
        extra_cycles++;
    }
};

void CPU::cpu_bmi() {
// - branch on result minus
    if (is_flag_set(negative_flag)) {
        // Page crossed
        if ((op_address >> 8) != (PC >> 8)) {
            extra_cycles += 2;
        }

        PC = op_address;
        extra_cycles++;
    }
};

void CPU::cpu_bne() {
// - branch on result not zero
    if (!is_flag_set(zero_flag)) {
        // Page crossed
        if ((op_address >> 8) != (PC >> 8)) {
            extra_cycles += 2;
        }

        PC = op_address;
        extra_cycles++;
    }
};

void CPU::cpu_bpl() {
// - branch on result plus
    if (!is_flag_set(negative_flag)) {
        // Page crossed
        if ((op_address >> 8) != (PC >> 8)) {
            extra_cycles += 2;
        }

        PC = op_address;
        extra_cycles++;
    }
};

void CPU::cpu_bvc() {
// - branch on overflow clear
    if (!is_flag_set(overflow_flag)) {
        // Page crossed
        if ((op_address >> 8) != (PC >> 8)) {
            extra_cycles += 2;
        }

        PC = op_address;
        extra_cycles++;
    }
};

void CPU::cpu_bvs() {
// - branch on overflow set
    if (is_flag_set(overflow_flag)) {
        // Page crossed
        if ((op_address >> 8) != (PC >> 8)) {
            extra_cycles += 2;
        }

        PC = op_address;
        extra_cycles++;
    }
};

//Jump
void CPU::cpu_jmp() {
// - jump to location
    PC = op_address;
};

void CPU::cpu_jsr() {
// - jump to location save return address
    word data = PC - 1;
    memory[0xff + SP] = data & 0xff;
    memory[0xff + SP + 1] = (data & 0xff00) >> 8;
    SP -= 2;
    PC = op_address;
};

void CPU::cpu_rti() {
// - return from interrupt

};

void CPU::cpu_rts() {
// - return from subroutine
    SP += 2;
    word from_stack = (memory[0xff + SP] | (memory[0xff + SP + 1] << 8));
    PC = from_stack + 1;
};

//Registers
void CPU::cpu_clc() {
// - clear carry flag
    P &= ~carry_flag;
};

void CPU::cpu_cld() {
// - clear decimal mode
    P &= ~decimal_flag;
};

void CPU::cpu_cli() {
// - clear interrupt disable bit
    P &= ~interrupt_flag;
};

void CPU::cpu_clv() {
// - clear overflow flag
    P &= ~overflow_flag;
};

void CPU::cpu_cmp() {
// - compare m and a
    if (A >= op_value) {
        P |= carry_flag;
    } else {
        P &= ~carry_flag;
    }

    if (A == op_value) {
        P |= zero_flag;
    } else {
        P &= ~zero_flag;
    }

    if (((A - op_value) >> 7) & 0x01) {
        P |= negative_flag;
    } else {
        P &= ~negative_flag;
    }
};

void CPU::cpu_cpx() {
// - compare m and x

};

void CPU::cpu_cpy() {
// - compare m and y

};

void CPU::cpu_sec() {
// - set carry flag
    P |= carry_flag;
};

void CPU::cpu_sed() {
// - set decimal mode
    P |= decimal_flag;
};

void CPU::cpu_sei() {
// - set interrupt disable status
    P |= interrupt_flag;
};

//Stack
void CPU::cpu_pha() {
// - push a on stack
    memory[0xff + SP] = A;
    SP -= 1;
};

void CPU::cpu_php() {
// - push processor status on stack
    byte data = P;
    memory[0xff + SP] = data | break_flag | unused_flag;
    SP -= 1;
};

void CPU::cpu_pla() {
// - pull a from stack
    SP += 1;
    byte from_stack = memory[0xff + SP];
    A = from_stack;

    if (A == 0) {
        P |= zero_flag;
    } else {
        P &= ~zero_flag;
    }

    if ((A >> 7) & 0x01) {
        P |= negative_flag;
    } else {
        P &= ~negative_flag;
    }
};

void CPU::cpu_plp() {
// - pull processor status from stack
    SP += 1;
    byte from_stack = memory[0xff + SP];
    P = (from_stack & ~break_flag) | unused_flag;
};

//System
void CPU::cpu_brk() {
// - force break

};

void CPU::cpu_nop() {
// - no operation
};

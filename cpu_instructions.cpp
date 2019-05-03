/*
 *  This file is based on the following reference: http://obelisk.me.uk/6502/reference.html
 *
 */

#include "include/cpu_instructions.h"
#include "include/cpu.h"

void Instruction::log_info() {
    std::cout << std::setw(2) << std::hex << (int)op_code << " ";
    std::cout << std::setw(4) << name << " ";
}

#define INST(_opc, _name, _bytes, _cycles, _mode) \
    if (_opc == op_code) { \
        memcpy(&result.name, #_name, 4); \
        result.bytes = _bytes; \
        result.cycles = _cycles; \
        result.func_ptr = &CPU::cpu_##_name; \
        result.address_func_ptr = &CPU::cpu_address_##_mode; \
    } \

Instruction CPU::get_instruction(byte op_code) {
    Instruction result;
    result.op_code = op_code;           // To see unimplemented OP code
    result.func_ptr = &CPU::cpu_nul;    // Prevent segfault if instruction not implemented
    result.address_func_ptr = &CPU::cpu_address_implied;

    INST(0xff, nul, 1, 1, implied);
    INST(0x00, brk, 1, 7, implied);
    INST(0x78, sei, 1, 2, implied);
    INST(0xd8, cld, 1, 2, implied);
    INST(0x9a, txs, 1, 2, implied);
    INST(0x10, bpl, 2, 2, relative);
    INST(0x20, jsr, 3, 6, absolute);
    INST(0x38, sec, 1, 2, implied);
    INST(0xb0, bcs, 2, 2, relative);
    INST(0x18, clc, 1, 2, implied);
    INST(0x58, cli, 1, 2, implied);
    INST(0xb8, clv, 1, 2, implied);
    INST(0x90, bcc, 2, 2, relative);
    INST(0xf0, beq, 2, 2, relative);
    INST(0xd0, bne, 2, 2, relative);
    INST(0x24, bit, 2, 3, zero_page);
    INST(0x2c, bit, 3, 4, absolute);
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
    INST(0xb6, ldx, 2, 4, zero_page_y);
    INST(0xae, ldx, 3, 4, absolute);
    INST(0xbe, ldx, 3, 4, absolute_y);

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
    INST(0xba, tsx, 1, 2, implied);
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
    INST(0x55, eor, 2, 4, zero_page_x);
    INST(0x4d, eor, 3, 4, absolute);
    INST(0x5d, eor, 3, 4, absolute_x);
    INST(0x59, eor, 3, 4, absolute_y);
    INST(0x41, eor, 2, 6, indirect_x);
    INST(0x51, eor, 2, 5, indirect_y);

    INST(0xe8, inx, 1, 2, implied);
    INST(0xc8, iny, 1, 2, implied);

    INST(0xe0, cpx, 2, 2, immediate);
    INST(0xe4, cpx, 2, 3, zero_page);
    INST(0xec, cpx, 3, 4, absolute);

    INST(0xc0, cpy, 2, 2, immediate);
    INST(0xc4, cpy, 2, 3, zero_page);
    INST(0xcc, cpy, 3, 4, absolute);

    INST(0x86, stx, 2, 3, zero_page);
    INST(0x96, stx, 2, 4, zero_page_y);
    INST(0x8e, stx, 3, 4, absolute);

    INST(0x84, sty, 2, 3, zero_page);
    INST(0x94, sty, 2, 4, zero_page_x);
    INST(0x8c, sty, 3, 4, absolute);

    INST(0x85, sta, 2, 3, zero_page);
    INST(0x95, sta, 2, 4, zero_page_x);
    INST(0x8d, sta, 3, 4, absolute);
    INST(0x9d, sta, 3, 5, absolute_x);
    INST(0x99, sta, 3, 5, absolute_y);
    INST(0x81, sta, 2, 6, indirect_x);
    INST(0x91, sta, 2, 6, indirect_y);

    INST(0xe6, inc, 2, 5, zero_page);
    INST(0xf6, inc, 2, 6, zero_page_x);
    INST(0xee, inc, 3, 6, absolute);
    INST(0xfe, inc, 3, 7, absolute_x);

    INST(0xca, dex, 1, 2, implied);
    INST(0x88, dey, 1, 2, implied);

    INST(0xc6, dec, 2, 5, zero_page);
    INST(0xd6, dec, 2, 6, zero_page_x);
    INST(0xce, dec, 3, 6, absolute);
    INST(0xde, dec, 3, 7, absolute_x);

    INST(0x0a, asla, 1, 2, implied);
    INST(0x06, asl,  2, 5, zero_page);
    INST(0x16, asl,  2, 6, zero_page_x);
    INST(0x0e, asl,  3, 6, absolute);
    INST(0x1e, asl,  3, 7, absolute_x);

    INST(0x4a, lsra, 1, 2, implied);
    INST(0x46, lsr,  2, 5, zero_page);
    INST(0x56, lsr,  2, 6, zero_page_x);
    INST(0x4e, lsr,  3, 6, absolute);
    INST(0x5e, lsr,  3, 7, absolute_x);

    INST(0x2a, rola, 1, 2, implied);
    INST(0x26, rol,  2, 5, zero_page);
    INST(0x36, rol,  2, 6, zero_page_x);
    INST(0x2e, rol,  3, 6, absolute);
    INST(0x3e, rol,  3, 7, absolute_x);

    INST(0x40, rti,  1, 6, implied);

    INST(0x69, adc, 2, 2, immediate);
    INST(0x65, adc, 2, 3, zero_page);
    INST(0x75, adc, 2, 4, zero_page_x);
    INST(0x6d, adc, 3, 4, absolute);
    INST(0x7d, adc, 3, 4, absolute_x);
    INST(0x79, adc, 3, 4, absolute_y);
    INST(0x61, adc, 2, 6, indirect_x);
    INST(0x71, adc, 2, 5, indirect_y);

    INST(0x6a, rora, 1, 2, implied);
    INST(0x66, ror,  2, 5, zero_page);
    INST(0x76, ror,  2, 6, zero_page_x);
    INST(0x6e, ror,  3, 6, absolute);
    INST(0x7e, ror,  3, 7, absolute_x);

    INST(0xe9, sbc, 2, 2, immediate);
    INST(0xe5, sbc, 2, 3, zero_page);
    INST(0xf5, sbc, 2, 4, zero_page_x);
    INST(0xed, sbc, 3, 4, absolute);
    INST(0xfd, sbc, 3, 4, absolute_x);
    INST(0xf9, sbc, 3, 4, absolute_y);
    INST(0xe1, sbc, 2, 6, indirect_x);
    INST(0xf1, sbc, 2, 5, indirect_y);
    INST(0xea, nop, 1, 2, implied);

    // Instructions bellow is unofficial
    INST(0x04, nop, 2, 3, zero_page);
    INST(0x44, nop, 2, 3, zero_page);
    INST(0x64, nop, 2, 3, zero_page);
    INST(0x0C, nop, 3, 4, absolute);

    INST(0x14, nop, 2, 4, zero_page_x);
    INST(0x34, nop, 2, 4, zero_page_x);
    INST(0x54, nop, 2, 4, zero_page_x);
    INST(0x74, nop, 2, 4, zero_page_x);
    INST(0xD4, nop, 2, 4, zero_page_x);
    INST(0xF4, nop, 2, 4, zero_page_x);

    INST(0x1a, nop, 1, 2, implied);
    INST(0x3a, nop, 1, 2, implied);
    INST(0x5a, nop, 1, 2, implied);
    INST(0x7a, nop, 1, 2, implied);
    INST(0xDa, nop, 1, 2, implied);
    INST(0xFa, nop, 1, 2, implied);

    INST(0x80, nop, 2, 2, immediate);

    INST(0x1c, nop, 3, 4, absolute_x);
    INST(0x3c, nop, 3, 4, absolute_x);
    INST(0x5c, nop, 3, 4, absolute_x);
    INST(0x7c, nop, 3, 4, absolute_x);
    INST(0xDc, nop, 3, 4, absolute_x);
    INST(0xFc, nop, 3, 4, absolute_x);

    return result;
}

#undef INST

void CPU::cpu_nul() {};

//Storage
void CPU::cpu_lda() {
// - load a with m
    A = op_value;

    update_zero_flag(A);
    update_negative_flag(A);
};

void CPU::cpu_ldx() {
// - load x with m
    X = op_value;

    update_zero_flag(X);
    update_negative_flag(X);
};

void CPU::cpu_ldy() {
// - load y with m
    Y = op_value;

    update_zero_flag(Y);
    update_negative_flag(Y);
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
    memory[op_address] = Y;
};

void CPU::cpu_tax() {
// - transfer a to x
    X = A;

    update_zero_flag(X);
    update_negative_flag(X);
};

void CPU::cpu_tay() {
// - transfer a to y
    Y = A;

    update_zero_flag(Y);
    update_negative_flag(Y);
};

void CPU::cpu_tsx() {
// - transfer stack pointer to x
    X = SP;

    update_zero_flag(X);
    update_negative_flag(X);
};

void CPU::cpu_txa() {
// - transfer x to a
    A = X;

    update_zero_flag(A);
    update_negative_flag(A);
};

void CPU::cpu_txs() {
// - transfer x to stack pointer
    SP = X;
};

void CPU::cpu_tya() {
// - transfer y to a
    A = Y;

    update_zero_flag(A);
    update_negative_flag(A);
};

//Math
void CPU::cpu_adc() {
// - add m to a with carry
    bool carry = (P & carry_flag);
    int res = A + op_value + carry;

    //Overflow, would happen if the sign of sum is different from BOTH operands
    update_overflow_flag((op_value ^ res) & (A ^ res) & 0x80);
    update_negative_flag(res);
    update_zero_flag(res & 0xFF);
    update_carry_flag(res & 0x100);

    A = res & 0xFF;
};

void CPU::cpu_dec() {
// - decrement m by one
    memory[op_address] -= 1;

    update_zero_flag(memory[op_address]);
    update_negative_flag(memory[op_address]);
};

void CPU::cpu_dex() {
// - decrement x by one
    X -= 1;

    update_zero_flag(X);
    update_negative_flag(X);
};

void CPU::cpu_dey() {
// - decrement y by one
    Y -= 1;

    update_zero_flag(Y);
    update_negative_flag(Y);
};

void CPU::cpu_inc() {
// - increment m by one
    memory[op_address] += 1;

    update_zero_flag(memory[op_address]);
    update_negative_flag(memory[op_address]);
};

void CPU::cpu_inx() {
// - increment x by one
    X += 1;

    update_zero_flag(X);
    update_negative_flag(X);
};

void CPU::cpu_iny() {
// - increment y by one
    Y += 1;

    update_zero_flag(Y);
    update_negative_flag(Y);
};

void CPU::cpu_sbc() {
// - subtract m from a with borrow
    bool carry = is_flag_set(carry_flag);
    int res = A - op_value - (1 - carry);

    update_overflow_flag((op_value ^ A) & (A ^ res) & 0x80); // FIXME: Magic
    update_carry_flag(!(res & 0x100)); // FIXME: Magic
    update_zero_flag(res);
    update_negative_flag(res & 0xFF);

    A = res & 0xFF;
};

//Bitwise
void CPU::cpu_and() {
// - "and" m with a
    A &= op_value;

    update_zero_flag(A);
    update_negative_flag(A);
};

void CPU::cpu_asl() {
// - shift left one bit (m or a)
    update_carry_flag((memory[op_address] >> 7) & 0x1);
    memory[op_address] <<= 1;
    update_zero_flag(op_value);
    update_negative_flag(op_value);
};

void CPU::cpu_asla() {
// - shift left one bit (m or a)
    update_carry_flag((A >> 7) & 0x1);
    A <<= 1;
    update_zero_flag(A);
    update_negative_flag(A);
};

void CPU::cpu_bit() {
// - test bits in m with a
    update_zero_flag(A & op_value);
    update_overflow_flag((op_value >> 6) & 0x1);
    update_negative_flag(op_value);
};

void CPU::cpu_eor() {
// - "exclusive-or" m with a
    A ^= op_value;

    update_zero_flag(A);
    update_negative_flag(A);
};

void CPU::cpu_lsr() {
// - shift right one bit (m or a)
    update_carry_flag(memory[op_address] & 0x1);
    memory[op_address] >>= 1;
    update_zero_flag(op_value >> 1);
    update_negative_flag(op_value);
};

void CPU::cpu_lsra() {
// - shift right one bit (m or a)
    update_carry_flag(A & 0x1);
    A >>= 1;
    update_zero_flag(A);
    update_negative_flag(A);
};

void CPU::cpu_ora() {
// - "or" m with a
    A |= op_value;

    update_zero_flag(A);
    update_negative_flag(A);
};

void CPU::cpu_rol() {
// - rotate one bit left (m or a)
    memory[op_address] <<= 1;
    memory[op_address] |= is_flag_set(carry_flag);

    update_carry_flag((op_value >> 7) & 0x1);
    update_zero_flag(memory[op_address]);
    update_negative_flag(memory[op_address]);
};

void CPU::cpu_rola() {
// - rotate one bit left (m or a)
    bool new_carry = (A >> 7) & 0x1;

    A <<= 1;
    A |= is_flag_set(carry_flag);

    update_carry_flag(new_carry);
    update_zero_flag(A);
    update_negative_flag(A);
};

void CPU::cpu_ror() {
// - rotate one bit right (m or a)
    bool carry = (P & carry_flag);
    update_carry_flag(op_value & 0x1);

    op_value = (op_value >> 1) | (carry << 7);

    update_zero_flag(op_value);
    update_negative_flag(op_value);

    memory[op_address] = op_value;
};

void CPU::cpu_rora() {
// - rotate one bit right (m or a)
    bool carry = (P & carry_flag);
    update_carry_flag(A & 0x1);

    A = (A >> 1) | (carry << 7);

    update_zero_flag(A);
    update_negative_flag(A);
};

//Branch
void CPU::cpu_bcc() {
// - branch on carry clear
    if (!is_flag_set(carry_flag)) {
        PC = op_address;
        extra_cycles++;
    }
};

void CPU::cpu_bcs() {
// - branch on carry set
    if (is_flag_set(carry_flag)) {
        PC = op_address;
        extra_cycles++;
    }
};

void CPU::cpu_beq() {
// - branch on result zero
    if (is_flag_set(zero_flag)) {
        PC = op_address;
        extra_cycles++;
    }
};

void CPU::cpu_bmi() {
// - branch on result minus
    if (is_flag_set(negative_flag)) {
        PC = op_address;
        extra_cycles++;
    }
};

void CPU::cpu_bne() {
// - branch on result not zero
    if (!is_flag_set(zero_flag)) {
        PC = op_address;
        extra_cycles++;
    }
};

void CPU::cpu_bpl() {
// - branch on result plus
    if (!is_flag_set(negative_flag)) {
        PC = op_address;
        extra_cycles++;
    }
};

void CPU::cpu_bvc() {
// - branch on overflow clear
    if (!is_flag_set(overflow_flag)) {
        PC = op_address;
        extra_cycles++;
    }
};

void CPU::cpu_bvs() {
// - branch on overflow set
    if (is_flag_set(overflow_flag)) {
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
    memory[0x0ff + SP] = data & 0xff;
    memory[0x0ff + SP + 1] = (data & 0xff00) >> 8;
    SP -= 2;
    PC = op_address;
};

void CPU::cpu_rti() {
// - return from interrupt
    SP += 1;
    P = memory[0x100 + SP] | unused_flag;

    SP += 2;
    PC = (memory[0x0ff + SP] | (memory[0x0ff + SP + 1] << 8));
};

void CPU::cpu_rts() {
// - return from subroutine
    SP += 2;
    word from_stack = (memory[0x0ff + SP] | (memory[0x0ff + SP + 1] << 8));
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
    word res = A - op_value;

    update_carry_flag(A >= op_value);
    update_zero_flag(res);
    update_negative_flag(res);
};

void CPU::cpu_cpx() {
// - compare m and x
    word res = X - op_value;

    update_carry_flag(X >= op_value);
    update_zero_flag(res);
    update_negative_flag(res);
};

void CPU::cpu_cpy() {
// - compare m and y
    word res = Y - op_value;

    update_carry_flag(Y >= op_value);
    update_zero_flag(res);
    update_negative_flag(res);
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
    memory[0x100 + SP] = A;
    SP -= 1;
};

void CPU::cpu_php() {
// - push processor status on stack
    byte data = P;
    memory[0x100 + SP] = data | break_flag | unused_flag;
    SP -= 1;
};

void CPU::cpu_pla() {
// - pull a from stack
    SP += 1;
    byte from_stack = memory[0x100 + SP];
    A = from_stack;

    update_zero_flag(A);
    update_negative_flag(A);
};

void CPU::cpu_plp() {
// - pull processor status from stack
    SP += 1;
    byte from_stack = memory[0x100 + SP];
    P = (from_stack & ~break_flag) | unused_flag;
};

//System
void CPU::cpu_brk() {
// - force break
    word data = PC - 1;
    memory[0x100 + SP] = data & 0xff;
    memory[0x100 + SP + 1] = (data & 0xff00) >> 8;
    SP -= 2;

    memory[0x100 + SP] = P;
    SP -= 1;

    P |= unused_flag | break_flag;

    //NMI
    byte low = memory[0xFFFA];
    byte high = memory[0xFFFA + 1];

    PC = low + (high << 8);
};

void CPU::cpu_nop() {
// - no operation
};

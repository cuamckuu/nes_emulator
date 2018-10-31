#ifndef CPU_H
#define CPU_H

#include <iostream>
#include <iomanip>
#include <cstdint>

#include "rom.h"
#include "cpu_instructions.h"

using byte = uint8_t;
using word = uint16_t;

struct CPU {
    byte memory[0x10000];

    word PC; // Program Counter,
    byte SP; // Stack Pointer,
    byte A, X, Y; // Registers
    byte P; // Flag Register

	long cpu_cycles = 0;
    long extra_cycles = 0;

    word op_address;
    byte op_value = 0x15;

    void load_rom_to_memory(ROM rom);
	void init();
	void reset();
	void log_state();
    void execute_instruction(Instruction nst);
    Instruction get_instruction(byte op_code);
	void run(long cycles, bool debug);
    bool is_flag_set(byte flag);

private: //CPU instructions
    void cpu_nul();

    //Storage
    void cpu_lda(); // - load a with m
    void cpu_ldx(); // - load x with m
    void cpu_ldy(); // - load y with m
    void cpu_sta(); // - store a in m
    void cpu_stx(); // - store x in m
    void cpu_sty(); // - store y in m
    void cpu_tax(); // - transfer a to x
    void cpu_tay(); // - transfer a to y
    void cpu_tsx(); // - transfer stack pointer to x
    void cpu_txa(); // - transfer x to a
    void cpu_txs(); // - transfer x to stack pointer
    void cpu_tya(); // - transfer y to a

    //Math
    void cpu_adc(); // - add m to a with carry
    void cpu_dec(); // - decrement m by one
    void cpu_dex(); // - decrement x by one
    void cpu_dey(); // - decrement y by one
    void cpu_inc(); // - increment m by one
    void cpu_inx(); // - increment x by one
    void cpu_iny(); // - increment y by one
    void cpu_sbc(); // - subtract m from a with borrow

    //Bitwise
    void cpu_and(); // - "and" m with a
    void cpu_asl(); // - shift left one bit (m or a)
    void cpu_bit(); // - test bits in m with a
    void cpu_eor(); // - "exclusive-or" m with a
    void cpu_lsr(); // - shift right one bit (m or a)
    void cpu_ora(); // - "or" m with a
    void cpu_rol(); // - rotate one bit left (m or a)
    void cpu_ror(); // - rotate one bit right (m or a)

    //Branch
    void cpu_bcc(); // - branch on carry clear
    void cpu_bcs(); // - branch on carry set
    void cpu_beq(); // - branch on result zero
    void cpu_bmi(); // - branch on result minus
    void cpu_bne(); // - branch on result not zero
    void cpu_bpl(); // - branch on result plus
    void cpu_bvc(); // - branch on overflow clear
    void cpu_bvs(); // - branch on overflow set

    //Jump
    void cpu_jmp(); // - jump to location
    void cpu_jsr(); // - jump to location save return address
    void cpu_rti(); // - return from interrupt
    void cpu_rts(); // - return from subroutine

    //Registers
    void cpu_clc(); // - clear carry flag
    void cpu_cld(); // - clear decimal mode
    void cpu_cli(); // - clear interrupt disable bit
    void cpu_clv(); // - clear overflow flag
    void cpu_cmp(); // - compare m and a
    void cpu_cpx(); // - compare m and x
    void cpu_cpy(); // - compare m and y
    void cpu_sec(); // - set carry flag
    void cpu_sed(); // - set decimal mode
    void cpu_sei(); // - set interrupt disable status

    //Stack
    void cpu_pha(); // - push a on stack
    void cpu_php(); // - push processor status on stack
    void cpu_pla(); // - pull a from stack
    void cpu_plp(); // - pull processor status from stack

    //System
    void cpu_brk(); // - force break
    void cpu_nop(); // - no operation

private: //CPU Adressing modes
    void cpu_address_immediate();
    void cpu_address_implied();
    void cpu_address_relative();

    void cpu_address_zero_page();
    void cpu_address_zero_page_x();
    void cpu_address_zero_page_y();

    void cpu_address_absolute();
    void cpu_address_absolute_x();
    void cpu_address_absolute_y();

    void cpu_address_indirect();
    void cpu_address_indirect_x();
    void cpu_address_indirect_y();

};

#endif //CPU_H

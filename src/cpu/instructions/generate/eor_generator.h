//
// Created by zoso on 11/20/24.
//

#ifndef EOR_GENERATOR_H
#define EOR_GENERATOR_H

#include "../instructions.h"
#include "binary_instruction_generator.h"

namespace cpu::instructions::generate {
    constexpr opcode_t OPCODE_EOR_EA   = 0b1011000100000000;
    constexpr opcode_t OPCODE_EORI     = 0b0000101000000000;
    constexpr opcode_t OPCODE_EORI2CCR = 0b0000101000111100;
    struct eor_generator {
        static void generate_eor(instructions_t& instructions) {
            binary_instruction_generator::generate_all_sizes<execute::eor_ea_handler>(instructions, OPCODE_EOR_EA);
        }

        static void generate_eori(instructions_t& instructions) {
            binary_instruction_generator::generate_all_sizes<execute::eori_handler>(instructions, OPCODE_EORI);
        }

        static void generate_eori2ccr(instructions_t& instructions) {
            instructions[OPCODE_EORI2CCR] = static_cast<instruction_fn>(instructions::execution_wrapper<execute::eori2ccr_handler, byte_t, ea::imm>);
        }
    };
}
#endif //EOR_GENERATOR_H

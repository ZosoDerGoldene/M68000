//
// Created by zoso on 11/19/24.
//

#ifndef AND_GENERATOR_H
#define AND_GENERATOR_H

#include "../instructions.h"
#include "binary_instruction_generator.h"

namespace cpu::instructions::generate {
    constexpr opcode_t OPCODE_AND_DN   = 0b1100000000000000;
    constexpr opcode_t OPCODE_AND_EA   = 0b1100000100000000;
    constexpr opcode_t OPCODE_ANDI     = 0b0000001000000000;
    constexpr opcode_t OPCODE_ANDI2CCR = 0b0000001000111100;
    struct and_generator {
        static void generate_and(instructions_t& instructions) {
            binary_instruction_generator::generate_all_sizes<execute::and_dn_handler>(instructions, OPCODE_AND_DN);
            binary_instruction_generator::generate_all_sizes<execute::and_ea_handler>(instructions, OPCODE_AND_EA);
        }

        static void generate_andi(instructions_t& instructions) {
            binary_instruction_generator::generate_all_sizes<execute::andi_handler>(instructions, OPCODE_ANDI);
        }

        static void generate_andi2ccr(instructions_t& instructions) {
            instructions[OPCODE_ANDI2CCR] = static_cast<instruction_fn>(
                instructions::execution_wrapper<execute::andi2ccr_handler, byte_t, ea::imm>);
        }
    };
}

#endif //AND_GENERATOR_H

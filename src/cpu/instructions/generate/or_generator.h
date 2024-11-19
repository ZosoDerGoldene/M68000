//
// Created by zoso on 11/19/24.
//

#ifndef OR_GENERATOR_H
#define OR_GENERATOR_H

#include "binary_instruction_generator.h"
#include "../instructions.h"

namespace cpu::instructions::generate {
    constexpr opcode_t OPCODE_OR_DN   = 0b1000000000000000;
    constexpr opcode_t OPCODE_OR_EA   = 0b1000000100000000;
    constexpr opcode_t OPCODE_ORI     = 0b0000000000000000;
    constexpr opcode_t OPCODE_ORI2CCR = 0b0000000000111100;

    struct or_generator {
        static void generate_or(instructions_t& instructions) {
            binary_instruction_generator::generate_all_sizes<execute::or_dn_handler>(instructions, OPCODE_OR_DN);
            binary_instruction_generator::generate_all_sizes<execute::or_ea_handler>(instructions, OPCODE_OR_EA);
        }

        static void generate_ori(instructions_t& instructions) {
            binary_instruction_generator::generate_all_sizes<execute::ori_handler>(instructions, OPCODE_ORI);
        }

        static void generate_ori2ccr(instructions_t& instructions) {
            instructions[OPCODE_ORI2CCR] = static_cast<instruction_fn>(
                instructions::execution_wrapper<execute::ori2ccr_handler, byte_t, ea::imm>);
        }
    };
}
#endif //OR_GENERATOR_H

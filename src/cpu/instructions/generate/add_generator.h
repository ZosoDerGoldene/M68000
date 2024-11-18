//
// Created by zoso on 11/18/24.
//

#ifndef ADD_GENERATOR_H
#define ADD_GENERATOR_H
#include "binary_instruction.h"
#include "../instructions.h"

namespace cpu::instructions::generate {

    constexpr opcode_t OPCODE_ADD_DN = 0b1101000000000000;
    constexpr opcode_t OPCODE_ADD_EA = 0b1101000100000000;

    struct add_generator {
        static void generate_add(instructions_t& instructions) {
            generate_binary_instruction::generate_all_sizes<execute::add_dn_handler>(instructions, OPCODE_ADD_DN);
            generate_binary_instruction::generate_all_sizes<execute::add_ea_handler>(instructions, OPCODE_ADD_EA);
        }
    };
}

#endif //ADD_GENERATOR_H

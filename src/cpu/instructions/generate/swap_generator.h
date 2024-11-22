//
// Created by zoso on 11/22/24.
//

#ifndef SWAP_GENERATOR_H
#define SWAP_GENERATOR_H

#include "unary_instruction_generator.h"
#include "../instructions.h"

namespace cpu::instructions::generate {
    constexpr opcode_t OPCODE_SWAP = 0b0100100001000000;

    struct swap_generator {
        static void generate_swap (instructions_t& instructions) {
            unary_instruction_generator::generate<execute::swap_handler, long_t>(instructions, OPCODE_SWAP);
        }
    };
}

#endif //SWAP_GENERATOR_H

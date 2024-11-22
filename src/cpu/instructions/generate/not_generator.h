//
// Created by zoso on 11/22/24.
//

#ifndef NOT_GENERATOR_H
#define NOT_GENERATOR_H

#include "../instructions.h"
#include "unary_instruction_generator.h"

namespace cpu::instructions::generate {
    constexpr opcode_t OPCODE_NOT = 0b0100011000000000;

    struct not_generator {
        static void generate_not (instructions_t& instructions) {
            unary_instruction_generator::generate_all_sizes<execute::not_handler>(instructions, OPCODE_NOT);
        }
    };
}


#endif //NOT_GENERATOR_H

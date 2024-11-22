//
// Created by zoso on 11/22/24.
//

#ifndef CLR_GENERATOR_H
#define CLR_GENERATOR_H
#include "../instructions.h"
#include "unary_instruction_generator.h"

namespace cpu::instructions::generate {

    constexpr opcode_t OPCODE_CLR = 0b0100001000000000;

    struct clr_generator {
        static void generate_clr(instructions_t& instructions) {
            unary_instruction_generator::generate_all_sizes<execute::clr_handler>(instructions, OPCODE_CLR);
        }
    };
}
#endif //CLR_GENERATOR_H

//
// Created by zoso on 11/22/24.
//

#ifndef TST_GENERATOR_H
#define TST_GENERATOR_H

#include "../instructions.h"
#include "unary_instruction_generator.h"

namespace cpu::instructions::generate {
    constexpr opcode_t OPCODE_TST = 0b0100101000000000;

    struct tst_generator {
        static void generate_tst (instructions_t& instructions) {
            unary_instruction_generator::generate_all_sizes<execute::tst_handler>(instructions, OPCODE_TST);
        }
    };
}

#endif //TST_GENERATOR_H

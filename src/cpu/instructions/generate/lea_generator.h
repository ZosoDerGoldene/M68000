//
// Created by zoso on 11/21/24.
//

#ifndef LEA_GENERATOR_H
#define LEA_GENERATOR_H

#include "../instructions.h"
#include "binary_instruction_generator.h"

namespace cpu::instructions::generate {
    constexpr opcode_t OPCODE_LEA = 0b0100000111000000;

    struct lea_generator {
        static inline void generate_lea(instructions_t& instructions) {
            binary_instruction_generator::generate<execute::lea_handler, long_t, long_t>(instructions, OPCODE_LEA);
        }
    };
}

#endif //LEA_GENERATOR_H

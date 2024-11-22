//
// Created by zoso on 11/22/24.
//

#ifndef NOP_GENERATOR_H
#define NOP_GENERATOR_H

#include "../instructions.h"

namespace cpu::instructions::generate {
    constexpr opcode_t OPCODE_NOP = 0b0100111001110001;
    struct nop_generator {
        static void generate_nop(instructions_t& instructions) {
            instructions[OPCODE_NOP] = static_cast<instruction_fn>(instructions::execution_wrapper<execute::nop_handler>);
        }
    };
}

#endif //NOP_GENERATOR_H

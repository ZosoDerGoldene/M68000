//
// Created by zoso on 11/19/24.
//

#ifndef MOVE_GENERATOR_H
#define MOVE_GENERATOR_H

#include "../instructions.h"
#include "binary_instruction_generator.h"
#include "unary_instruction_generator.h"

namespace cpu::instructions::generate {
    constexpr opcode_t OPCODE_MOVE_B  = 0b0001000000000000;
    constexpr opcode_t OPCODE_MOVE_W  = 0b0011000000000000;
    constexpr opcode_t OPCODE_MOVE_L  = 0b0010000000000000;
    constexpr opcode_t OPCODE_MOVEA_W = 0b0011000001000000;   /*
*                                                                      TODO: Verify the size field for movea is
                                                                actually switched up from move.w/move.l
                                                            */
    constexpr opcode_t OPCODE_MOVEA_L = 0b0010000001000000;
    constexpr opcode_t OPCODE_MOVEQ   = 0b0111000000000000;

    struct move_generator {
        static inline void generate_move(instructions_t& instructions) {
            binary_instruction_generator::generate<execute::move_handler, byte_t, byte_t>(instructions, OPCODE_MOVE_B);
            binary_instruction_generator::generate<execute::move_handler, word_t, word_t>(instructions, OPCODE_MOVE_W);
            binary_instruction_generator::generate<execute::move_handler, long_t, long_t>(instructions, OPCODE_MOVE_L);
        }

        static inline void generate_movea(instructions_t& instructions) {
            binary_instruction_generator::generate<execute::movea_handler, word_t, word_t>(instructions, OPCODE_MOVEA_W);
            binary_instruction_generator::generate<execute::movea_handler, long_t, long_t>(instructions, OPCODE_MOVEA_L);
        }

        static inline void generate_moveq(instructions_t& instructions) {
            for (opcode_t i = 0; i < 0x100; i++) {
                unary_instruction_generator::generate<execute::moveq_handler, long_t>(instructions, OPCODE_MOVEQ | i);
            }
        }

    };
}

#endif //MOVE_GENERATOR_H

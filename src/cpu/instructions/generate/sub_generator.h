//
// Created by zoso on 11/19/24.
//

#ifndef SUB_GENERATOR_H
#define SUB_GENERATOR_H

#include "binary_instruction_generator.h"
#include "unary_instruction_generator.h"
#include "x_instrcution_generator.h"
#include "../instructions.h"

namespace cpu::instructions::generate {
    constexpr opcode_t OPCODE_SUB_DN = 0b1001000000000000;
    constexpr opcode_t OPCODE_SUB_EA = 0b1001000100000000;
    constexpr opcode_t OPCODE_SUBA_W = 0b1001000010000000;
    constexpr opcode_t OPCODE_SUBA_L = 0b1001000110000000;
    constexpr opcode_t OPCODE_SUBI   = 0b0000010000000000;
    constexpr opcode_t OPCODE_SUBX   = 0b1001000100000000;
    constexpr opcode_t OPCODE_SUBQ   = 0b0101000100000000;

    struct sub_generator {
        static void generate_sub(instructions_t &instructions) {
            binary_instruction_generator::generate_all_sizes<execute::sub_dn_handler>(instructions, OPCODE_SUB_DN);
            binary_instruction_generator::generate_all_sizes<execute::sub_ea_handler>(instructions, OPCODE_SUB_EA);
        }

        static void generate_suba(instructions_t &instructions) {
            binary_instruction_generator::generate<execute::suba_handler, word_t, word_t>(instructions, OPCODE_SUBA_W);
            binary_instruction_generator::generate<execute::suba_handler, long_t, long_t>(instructions, OPCODE_SUBA_L);
        }

        static void generate_subi(instructions_t &instructions) {
            binary_instruction_generator::generate_all_sizes<execute::subi_handler>(instructions, OPCODE_SUBI);
        }

        static void generate_subq(instructions_t &instructions) {
            for (opcode_t i = 0; i < 8; i++) {
                unary_instruction_generator::generate_all_sizes<execute::subq_handler>(instructions, OPCODE_SUBQ | (i << 9));
            }
        }

        static void generate_subx(instructions_t &instructions) {
            x_instruction_generator::generate<execute::subx_handler>(instructions, OPCODE_SUBX);
        }

    };
}
#endif //SUB_GENERATOR_H

//
// Created by zoso on 11/18/24.
//

#ifndef ADD_GENERATOR_H
#define ADD_GENERATOR_H
#include "binary_instruction_generator.h"
#include "unary_instruction_generator.h"
#include "x_instruction_generator.h"
#include "../instructions.h"

namespace cpu::instructions::generate {

    constexpr opcode_t OPCODE_ADD_DN = 0b1101000000000000;
    constexpr opcode_t OPCODE_ADD_EA = 0b1101000100000000;
    constexpr opcode_t OPCODE_ADDA_W = 0b1101000011000000;
    constexpr opcode_t OPCODE_ADDA_L = 0b1101000111000000;
    constexpr opcode_t OPCODE_ADDI   = 0b0000011000000000;
    constexpr opcode_t OPCODE_ADDX   = 0b1101000100000000;
    constexpr opcode_t OPCODE_ADDQ   = 0b0101000000000000;

    struct add_generator {
        static void generate_add(instructions_t& instructions) {
            binary_instruction_generator::generate_all_sizes<execute::add_dn_handler>(instructions, OPCODE_ADD_DN);
            binary_instruction_generator::generate_all_sizes<execute::add_ea_handler>(instructions, OPCODE_ADD_EA);
        }
        
        static void generate_adda(instructions_t& instructions) {
            binary_instruction_generator::generate<execute::adda_handler, word_t, word_t>(instructions, OPCODE_ADDA_W);
            binary_instruction_generator::generate<execute::adda_handler, long_t, long_t>(instructions, OPCODE_ADDA_L);
        }

        static void generate_addi(instructions_t& instructions) {
            binary_instruction_generator::generate_all_sizes<execute::addi_handler>(instructions, OPCODE_ADDI);
        }

        static void generate_addq(instructions_t &instructions) {
            for (opcode_t i = 0; i < 8; i++) {
                unary_instruction_generator::generate_all_sizes<execute::addq_handler>(instructions, OPCODE_ADDQ | (i << 9));
            }
        }

        static void generate_addx(instructions_t& instructions) {
            x_instruction_generator::generate<execute::addx_handler>(instructions, OPCODE_ADDX);
        }
    };
}

#endif //ADD_GENERATOR_H

//
// Created by zoso on 11/22/24.
//

#ifndef CMP_GENERATOR_H
#define CMP_GENERATOR_H

#include "../instructions.h"
#include "binary_instruction_generator.h"



namespace cpu::instructions::generate {
    constexpr opcode_t OPCODE_CMP    = 0b1011000000000000;
    constexpr opcode_t OPCODE_CMPA_W = 0b1011000011000000;
    constexpr opcode_t OPCODE_CMPA_L = 0b1011000111000000;
    constexpr opcode_t OPCODE_CMPI   = 0b0000110000000000;

    struct cmp_generator {
        static void generate_cmp(instructions_t &instructions) {
            binary_instruction_generator::generate_all_sizes<execute::cmp_handler>(instructions, OPCODE_CMP);
        }

        static void generate_cmpa(instructions_t &instructions) {
            binary_instruction_generator::generate<execute::cmpa_handler, word_t, word_t>(instructions, OPCODE_CMPA_W);
            binary_instruction_generator::generate<execute::cmpa_handler, long_t, long_t>(instructions, OPCODE_CMPA_L);
        }

        static void generate_cmpi(instructions_t &instructions) {
            binary_instruction_generator::generate_all_sizes<execute::cmpi_handler>(instructions, OPCODE_CMPI);
        }
    };
}

#endif //CMP_GENERATOR_H

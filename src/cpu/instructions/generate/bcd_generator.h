//
// Created by zoso on 11/20/24.
//

#ifndef BCD_GENERATOR_H
#define BCD_GENERATOR_H

#include "../instructions.h"
#include "x_instruction_generator.h"
namespace cpu::instructions::generate {
    
            constexpr opcode_t OPCODE_ABCD    = 0b1100000100000000;
            constexpr opcode_t OPCODE_SBCD    = 0b1000000100000000;
            constexpr opcode_t OPCODE_PACK_DN = 0b1000000101000000;
            constexpr opcode_t OPCODE_PACK_AN = 0b1000000101001000;
            constexpr opcode_t OPCODE_UNPK_DN = 0b1000000110000000;
            constexpr opcode_t OPCODE_UNPK_AN = 0b1000000110001000;

            struct bcd_generator {
                static void generate_abcd(instructions_t& instructions) {
                    x_instruction_generator::generate<execute::abcd_handler, true>(instructions, OPCODE_ABCD);
                }

                static void generate_sbcd(instructions_t& instructions) {
                    x_instruction_generator::generate<execute::sbcd_handler, true>(instructions, OPCODE_SBCD);
                }

                static void generate_pack(instructions_t& instructions) {
                    for (opcode_t i = 0; i < 8; i++) {
                        for (opcode_t j = 0; j < 8; j++) {
                            instructions[OPCODE_PACK_DN | (i<<9) | j] = static_cast<instruction_fn>
                                (execute::pack_unpack_handler::execute<execute::pack_handler, ea::dn>);
                        }
                    }
                    for (opcode_t i = 0; i < 8; i++) {
                        for (opcode_t j = 0; j < 8; j++) {
                            instructions[OPCODE_PACK_AN | (i<<9) | j] = static_cast<instruction_fn>
                                (execute::pack_unpack_handler::execute<execute::pack_handler, ea::an_ind_pre_decr>);
                        }
                    }
                }

                static void generate_unpk(instructions_t& instructions) {
                    for (opcode_t i = 0; i < 8; i++) {
                        for (opcode_t j = 0; j < 8; j++) {
                            instructions[OPCODE_UNPK_DN | (i<<9) | j] = static_cast<instruction_fn>
                                (execute::pack_unpack_handler::execute<execute::unpk_handler, ea::dn>);
                        }
                    }
                    for (opcode_t i = 0; i < 8; i++) {
                        for (opcode_t j = 0; j < 8; j++) {
                            instructions[OPCODE_UNPK_AN | (i<<9) | j] = static_cast<instruction_fn>
                                (execute::pack_unpack_handler::execute<execute::unpk_handler, ea::an_ind_pre_decr>);
                        }
                    }
                }
            };
}
#endif //BCD_GENERATOR_H

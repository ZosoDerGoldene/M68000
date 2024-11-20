//
// Created by zoso on 11/19/24.
//

#ifndef X_INSTRCUTION_GENERATOR_H
#define X_INSTRCUTION_GENERATOR_H

#include "../instructions.h"
namespace cpu::instructions::generate {
           struct x_instruction_generator {
                template<typename implementation, bool byte_only = false, bool ignore_source = false, bool dn_only
                    = false>
                static void generate(instructions_t& instructions, const opcode_t base_opcode) {
                    for (opcode_t i = 0; i < 8; i++) {
                        for(opcode_t j = 0; j < 8; j++) {
                            if constexpr (ignore_source) {
                                instructions[base_opcode | (i << 9) | j] =
                                    static_cast<instruction_fn>(instructions::execution_wrapper<implementation, byte_t, ea::dn>);
                                if constexpr(!byte_only) {
                                    instructions[base_opcode | 0b1000000 | (i << 9) | j] =
                                        static_cast<instruction_fn>(instructions::execution_wrapper<implementation, word_t, ea::dn>);
                                    instructions[base_opcode | 0b10000000 | (i << 9) | j] =
                                        static_cast<instruction_fn>(instructions::execution_wrapper<implementation, long_t, ea::dn>);
                                }
                            } else {
                                instructions[base_opcode | (i << 9) | j] =
                                    static_cast<instruction_fn>(instructions::execution_wrapper<implementation, byte_t, byte_t, ea::dn, ea::dn>);
                                if constexpr( !dn_only) {
                                    instructions[base_opcode | 0b1000 | (i << 9) | j] =
                                        static_cast<instruction_fn>(instructions::execution_wrapper<implementation, byte_t, byte_t, ea::an_ind_pre_decr, ea::an_ind_pre_decr>);
                                }
                                if constexpr(!byte_only) {
                                    instructions[base_opcode | 0b1000000 | (i << 9) | j] =
                                        static_cast<instruction_fn>(instructions::execution_wrapper<implementation, word_t, word_t, ea::dn, ea::dn>);
                                    instructions[base_opcode | 0b10000000 | (i << 9) | j] =
                                        static_cast<instruction_fn>(instructions::execution_wrapper<implementation, long_t, long_t, ea::dn, ea::dn>);
                                    if constexpr (!dn_only) {
                                        instructions[base_opcode | 0b1001000 | (i << 9) | j] =
                                            static_cast<instruction_fn>(instructions::execution_wrapper<implementation, word_t, word_t, ea::an_ind_pre_decr, ea::an_ind_pre_decr>);
                                        instructions[base_opcode | 0b10001000 | (i << 9) | j] =
                                            static_cast<instruction_fn>(instructions::execution_wrapper<implementation, long_t, long_t, ea::an_ind_pre_decr, ea::an_ind_pre_decr>);
                                    }
                                }
                            }
                        }
                    }
                }
            };
}
#endif //X_INSTRCUTION_GENERATOR_H

//
// Created by zoso on 11/19/24.
//

#ifndef UNARY_INSTRUCTION_GENERATOR_H
#define UNARY_INSTRUCTION_GENERATOR_H

#include "../instructions.h"

namespace cpu::instructions::generate {
    struct unary_instruction_generator {
            public:
                template<typename handler>
                static inline void generate_all_sizes(instructions_t& instructions, const opcode_t base_opcode) {
                    generate<handler, byte_t>(instructions, base_opcode);
                    generate<handler, word_t>(instructions, base_opcode | (1 << 6));
                    generate<handler, long_t>(instructions, base_opcode | (1 << 7));
                }

                template<typename handler, unsigned_integer dest_t>
                static inline void generate(instructions_t& instructions, const opcode_t base_opcode) {
                    generate_impl<handler, dest_t, ea::dn>(instructions, base_opcode);
                    if constexpr (!is_byte<dest_t>) {
                        generate_impl<handler, dest_t, ea::an>(instructions, base_opcode);
                    }
                    generate_impl<handler, dest_t, ea::an_ind>(instructions, base_opcode);
                    generate_impl<handler, dest_t, ea::an_ind_post_incr>(instructions, base_opcode);
                    generate_impl<handler, dest_t, ea::an_ind_pre_decr>(instructions, base_opcode);
                    generate_impl<handler, dest_t, ea::an_ind_disp>(instructions, base_opcode);
                    generate_impl<handler, dest_t, ea::an_ind_idx<true>>(instructions, base_opcode);
                    generate_impl<handler, dest_t, ea::abs_short>(instructions, base_opcode);
                    generate_impl<handler, dest_t, ea::abs_long>(instructions, base_opcode);
                    generate_impl<handler, dest_t, ea::pc_ind_disp>(instructions, base_opcode);
                    generate_impl<handler, dest_t, ea::pc_ind_idx<true>>(instructions, base_opcode);
                    generate_impl<handler, dest_t, ea::imm>(instructions, base_opcode);
                }

            private:
                template<typename handler, typename dst_mode, typename container_t>
                static inline void get_indices(opcode_t base_opcode, container_t& values) {
                    if constexpr (std::is_same_v<container_t, std::array<opcode_t, 8>>) {
                        const std::array<opcode_t, 8> ms = ea::mode_masks<dst_mode, execute::reverses_src_dst<handler>>();
                        for (int i = 0; i < 8; i++) {
                            values[i] = base_opcode | ms[i];
                        }
                    } else {
                        values[0] = base_opcode | ea::mode_mask<dst_mode, execute::reverses_src_dst<handler>>() ;
                    }
                }

                template<typename handler, unsigned_integer dst_t, typename dst_mode, typename container_t>
                static inline void fill_impl(instructions_t& instructions, const opcode_t base_opcode) {
                    container_t values = container_t();
                    get_indices<handler, dst_mode, container_t>(base_opcode, values);
                    for (size_t i = 0; i < values.size(); i++) {
                        instructions[values[i]] = static_cast<instruction_fn>(instructions::execution_wrapper<handler,
                            dst_t, dst_mode>);
                    }
                }

                template<typename handler, unsigned_integer dst_t, typename dst_mode>
                static inline void fill(instructions_t& instructions, const opcode_t base_opcode) {
                    if constexpr (ea::register_mode<dst_mode>) {
                        fill_impl<handler, dst_t, dst_mode, std::array<opcode_t, 8>>(instructions, base_opcode);
                    } else {
                        fill_impl<handler, dst_t, dst_mode, std::array<opcode_t, 1>>(instructions, base_opcode);
                    }
                }

                template< typename handler, unsigned_integer dst_t,typename dst_mode>
                static inline void generate_impl(instructions_t& instructions, const opcode_t base_opcode) {
                    if constexpr (handler::template is_valid_destination<dst_mode>) {
                        fill<handler, dst_t, dst_mode>(instructions, base_opcode);
                    }
                }
            };
}
#endif //UNARY_INSTRUCTION_GENERATOR_H

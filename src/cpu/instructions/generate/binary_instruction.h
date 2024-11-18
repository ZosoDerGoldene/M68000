//
// Created by zoso on 11/18/24.
//

#ifndef BINARY_INSTRUCTION_H
#define BINARY_INSTRUCTION_H
#include <array>

#include "../instructions.h"
#include "../execute/execution_handlers.h"

namespace cpu::instructions::generate {
    
        struct generate_binary_instruction {
        template<typename handler>
        static void generate_all_sizes(instructions_t& instructions, const opcode_t base_opcode) {
            generate<handler, byte_t, byte_t>(instructions, base_opcode);
            generate<handler, word_t, word_t>(instructions, base_opcode | (1<<6));
            generate<handler, long_t, long_t>(instructions, base_opcode | (1<<7));
        }

        template<typename handler, integer src_t, integer dst_t>
        static void generate(instructions_t& instructions, const opcode_t base_opcode) {
            generate_impl< handler, src_t, dst_t, ea::dn>(instructions, base_opcode);
            if constexpr ((!is_byte<src_t> && (!is_byte<dst_t>))) {
                generate_impl<handler, src_t, dst_t, ea::an>(instructions, base_opcode);
            }
            generate_impl<handler, src_t, dst_t, ea::an_ind>(instructions, base_opcode);
            generate_impl<handler, src_t, dst_t, ea::an_ind_post_incr>(instructions, base_opcode);
            generate_impl<handler, src_t, dst_t, ea::an_ind_pre_decr>(instructions, base_opcode);
            generate_impl<handler, src_t, dst_t, ea::an_ind_disp>(instructions, base_opcode);
            generate_impl<handler, src_t, dst_t, ea::an_ind_idx<true>>(instructions, base_opcode);
            generate_impl<handler, src_t, dst_t, ea::abs_short>(instructions, base_opcode);
            generate_impl<handler, src_t, dst_t, ea::abs_long>(instructions, base_opcode);
            generate_impl<handler, src_t, dst_t, ea::pc_ind_disp>(instructions, base_opcode);
            generate_impl<handler, src_t, dst_t, ea::pc_ind_idx<true>>(instructions, base_opcode);
            generate_impl<handler, src_t, dst_t, ea::imm>(instructions, base_opcode);
        }
    private:

        template<typename handler, ea::ea_mode src_mode, ea::ea_mode dst_mode, typename container_t>
        static inline void get_indices(const opcode_t base_opcode, container_t& values) {
            if constexpr (std::is_same_v<container_t, std::array<opcode_t, 64>>) {
                std::array<opcode_t, 8> src_modes = ea::mode_masks<src_mode, !execute::reverses_src_dst<handler>>();
                std::array<opcode_t, 8> dst_modes = ea::mode_masks<dst_mode, execute::reverses_src_dst<handler>>();
                for (unsigned int i = 0; i < 8; i++) {
                    for (unsigned int j = 0; j < 8; j++) {
                        values[i * 8 + j] = base_opcode | src_modes[i] | dst_modes[j];
                    }
                }
            } else if constexpr (std::is_same_v<container_t, std::array<opcode_t, 8>>) {
                std::array<opcode_t, 8> ms{};
                opcode_t m;
                if constexpr (ea::register_mode<src_mode>) {
                    ms = ea::mode_masks<src_mode, !execute::reverses_src_dst<handler>>();
                    m = ea::mode_mask<dst_mode, execute::reverses_src_dst<handler>>();
                } else {
                    if constexpr (execute::is_immediate_instruction<handler>) {
                        m = 0;
                    } else {
                        m = ea::mode_mask<src_mode, !execute::reverses_src_dst<handler>>();
                    }
                    ms = ea::mode_masks<dst_mode, execute::is_immediate_instruction<handler>>();
                }
                for (unsigned int i = 0; i < 8; i++) {
                    values[i] = base_opcode | m | ms[i];
                }
            } else {
                if constexpr (execute::is_immediate_instruction<handler>) {
                    values[0] = base_opcode | ea::mode_mask<dst_mode, !execute::reverses_src_dst<handler>>();
                } else {
                    values[0] = base_opcode | ea::mode_mask<src_mode, !execute::reverses_src_dst<handler>>() |
                    ea::mode_mask<dst_mode, execute::reverses_src_dst<handler>>();
                }
            }
        }

        template<typename handler, integer src_t, integer dst_t, ea::ea_mode src_mode, ea::ea_mode dst_mode,
            typename container>
        static inline void fill_impl(instructions_t& instructions, const opcode_t base_opcode) {
            container values = container();
            get_indices<handler, src_mode, dst_mode, container>(base_opcode, values);
            for (size_t i = 0; i < values.size(); i++) {
                assert(instructions[values[i]] == nullptr);
                instructions[values[i]] = static_cast<instruction_fn>(instructions::execution_wrapper<handler, src_t, dst_t,
                    src_mode, dst_mode>);
            }
        }

        template<typename handler, integer src_t, integer dst_t, ea::ea_mode src_mode, ea::ea_mode dst_mode>
        static inline void fill_do(instructions_t & instructions, const opcode_t base_opcode) {
            if constexpr (ea::register_mode<src_mode> && ea::register_mode<dst_mode>) {
                fill_impl<handler, src_t, dst_t, src_mode, dst_mode, std::array<opcode_t, 64>>(instructions, base_opcode);
            } else if constexpr (ea::register_mode<src_mode> || ea::register_mode<dst_mode>) {
                fill_impl<handler, src_t, dst_t, src_mode, dst_mode, std::array<opcode_t, 8>>(instructions, base_opcode);
            } else {
                fill_impl<handler, src_t, dst_t, src_mode, dst_mode, std::array<opcode_t, 1>>(instructions, base_opcode);
            }
        }

        template<typename handler, integer src_t, integer dst_t, ea::ea_mode src_mode, ea::ea_mode dst_mode>
        static inline void fill(instructions_t& instructions, const opcode_t base_opcode) {
            if constexpr (execute::is_bit_instruction<handler>) {
                if constexpr (handler::template is_valid<dst_mode, false, dst_t>()) {
                    fill_do<handler, src_t, dst_t, src_mode, dst_mode>(instructions, base_opcode);
                }
            } else if constexpr (handler::template is_valid_destination<dst_mode>) {
                fill_do<handler, src_t, dst_t, src_mode, dst_mode>(instructions, base_opcode);
            }
        }

        template<typename handler, integer src_t, integer dst_t, ea::ea_mode src_mode>
        static inline void generate_impl_do(instructions_t& instructions, const opcode_t base_opcode) {
            fill<handler, src_t, dst_t, src_mode, ea::dn>(instructions, base_opcode);
            if constexpr ((!is_byte<src_t> && (!is_byte<dst_t>))) {
                fill<handler, src_t, dst_t, src_mode, ea::an>(instructions, base_opcode);
            }
            fill<handler, src_t, dst_t, src_mode, ea::an_ind>(instructions, base_opcode);
            fill<handler, src_t, dst_t, src_mode, ea::an_ind_post_incr>(instructions, base_opcode);
            fill<handler, src_t, dst_t, src_mode, ea::an_ind_pre_decr>(instructions, base_opcode);
            fill<handler, src_t, dst_t, src_mode, ea::an_ind_disp>(instructions, base_opcode);
            fill<handler, src_t, dst_t, src_mode, ea::an_ind_idx<true>>(instructions, base_opcode);
            fill<handler, src_t, dst_t, src_mode, ea::abs_short>(instructions, base_opcode);
            fill<handler, src_t, dst_t, src_mode, ea::abs_long>(instructions, base_opcode);
        }

        template<typename handler, integer src_t, integer dst_t, ea::ea_mode src_mode>
        static inline void generate_impl(instructions_t& instructions, const opcode_t base_opcode) {
            if constexpr (execute::is_bit_instruction<handler>) {
                if constexpr (handler::template is_valid<src_mode, true, src_t>()) {
                    generate_impl_do<handler, src_t, dst_t, src_mode>(instructions, base_opcode);
                }
            } else if constexpr (handler::template is_valid_source<src_mode>) {
                generate_impl_do<handler, src_t, dst_t, src_mode>(instructions, base_opcode);
            }
        }
    };
}

#endif //BINARY_INSTRUCTION_H

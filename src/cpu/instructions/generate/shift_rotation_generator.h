//
// Created by zoso on 11/19/24.
//

#ifndef SHIFT_ROTATION_GENERATOR_H
#define SHIFT_ROTATION_GENERATOR_H

#include "unary_instruction_generator.h"
#include "x_instruction_generator.h"
#include "../instructions.h"

namespace cpu::instructions::generate {
    constexpr opcode_t OPCODE_BASE               = 0b1110000000000000;
    constexpr opcode_t OPCODE_LEFT               = 0b0000000100000000;
    constexpr opcode_t OPCODE_DN2DN              = 0b0000000000100000;
    constexpr opcode_t OPCODE_AS                 = 0b0000000000000000;
    constexpr opcode_t OPCODE_LS                 = 0b0000000000001000;
    constexpr opcode_t OPCODE_RO                 = 0b0000000000011000;
    constexpr opcode_t OPCODE_ROX                = 0b0000000000010000;
    constexpr opcode_t OPCODE_AS_EA              = 0b0000000011000000;
    constexpr opcode_t OPCODE_LS_EA              = 0b0000001011000000;
    constexpr opcode_t OPCODE_RO_EA              = 0b0000011011000000;
    constexpr opcode_t OPCODE_ROX_EA             = 0b0000010011000000;

    struct asx_generator {
        static void generate_asx(instructions_t& instructions) {
            generate_asx_dn(instructions);
            generate_asx_implicit(instructions);
            generate_asx_ea(instructions);
        }

        static void generate_asx_dn(instructions_t& instructions) {
            x_instruction_generator::generate<execute::asx_dn_handler<false>, false, false, true>(instructions, OPCODE_BASE | OPCODE_AS | OPCODE_DN2DN);
            x_instruction_generator::generate<execute::asx_dn_handler<true>, false, false, true>(instructions, OPCODE_BASE | OPCODE_AS | OPCODE_DN2DN | OPCODE_LEFT);
        }

        static void generate_asx_implicit(instructions_t& instructions) {
            x_instruction_generator::generate<execute::asx_implicit_handler<false>, false, true, true>(instructions, OPCODE_BASE | OPCODE_AS);
            x_instruction_generator::generate<execute::asx_implicit_handler<true>, false, true, true>(instructions, OPCODE_BASE | OPCODE_AS | OPCODE_LEFT);
        }

        static void generate_asx_ea(instructions_t& instructions) {
            unary_instruction_generator::generate<execute::asx_ea_handler<false>, word_t>(instructions, OPCODE_BASE | OPCODE_AS_EA);
            unary_instruction_generator::generate<execute::asx_ea_handler<true>, word_t>(instructions, OPCODE_BASE | OPCODE_LEFT | OPCODE_AS_EA);
        }
    };

    struct lsx_generator {
        static void generate_lsx(instructions_t& instructions) {
            generate_lsx_dn(instructions);
            generate_lsx_implicit(instructions);
            generate_lsx_ea(instructions);
        }

        static void generate_lsx_dn(instructions_t& instructions) {
            x_instruction_generator::generate<execute::lsx_dn_handler<false>, false, false, true>(instructions, OPCODE_BASE | OPCODE_LS | OPCODE_DN2DN);
            x_instruction_generator::generate<execute::lsx_dn_handler<true>, false, false, true>(instructions, OPCODE_BASE | OPCODE_LS | OPCODE_DN2DN | OPCODE_LEFT);
        }

        static void generate_lsx_implicit(instructions_t& instructions) {
            x_instruction_generator::generate<execute::lsx_implicit_handler<false>, false, true, true>(instructions, OPCODE_BASE | OPCODE_LS );
            x_instruction_generator::generate<execute::lsx_implicit_handler<true>, false, true, true>(instructions, OPCODE_BASE | OPCODE_LS | OPCODE_LEFT);}

        static void generate_lsx_ea(instructions_t& instructions) {
            unary_instruction_generator::generate<execute::lsx_ea_handler<false>, word_t>(instructions, OPCODE_BASE | OPCODE_LS_EA );
            unary_instruction_generator::generate<execute::lsx_ea_handler<true>, word_t>(instructions, OPCODE_BASE | OPCODE_LEFT | OPCODE_LS_EA) ;
        }
    };

    struct rox_generator {
        static void generate_rox(instructions_t& instructions) {
            generate_rox_dn(instructions);
            generate_rox_implicit(instructions);
            generate_rox_ea(instructions);
        }
        static void generate_rox_dn(instructions_t& instructions) {
            x_instruction_generator::generate<execute::rox_dn_handler<false, false>, false, false, true>(instructions, OPCODE_BASE | OPCODE_RO | OPCODE_DN2DN);
            x_instruction_generator::generate<execute::rox_dn_handler<true, false>, false, false, true>(instructions, OPCODE_BASE | OPCODE_RO | OPCODE_DN2DN | OPCODE_LEFT);
        }
        static void generate_rox_implicit(instructions_t& instructions) {
            x_instruction_generator::generate<execute::rox_implicit_handler<false, false>, false, true, true>(instructions, OPCODE_BASE | OPCODE_RO );
            x_instruction_generator::generate<execute::rox_implicit_handler<true, false>, false, true, true>(instructions, OPCODE_BASE | OPCODE_RO |
            OPCODE_LEFT);
        }
        static void generate_rox_ea(instructions_t& instructions) {
            unary_instruction_generator::generate<execute::rox_ea_handler<false, false>, word_t>(instructions, OPCODE_BASE | OPCODE_RO_EA);
            unary_instruction_generator::generate<execute::rox_ea_handler<true, false>, word_t>(instructions, OPCODE_BASE | OPCODE_LEFT | OPCODE_RO_EA);
        }
    };

    struct roxx_generator {
        static void generate_roxx(instructions_t& instructions) {
            generate_roxx_dn(instructions);
            generate_roxx_implicit(instructions);
            generate_roxx_ea(instructions);
        }

        static void generate_roxx_dn(instructions_t& instructions) {
            x_instruction_generator::generate<execute::rox_dn_handler<false, true>, false, false, true>(instructions, OPCODE_BASE | OPCODE_ROX | OPCODE_DN2DN);
            x_instruction_generator::generate<execute::rox_dn_handler<true, true>, false, false, true>(instructions, OPCODE_BASE | OPCODE_ROX | OPCODE_DN2DN | OPCODE_LEFT);
        }
        static void generate_roxx_implicit(instructions_t& instructions) {
            x_instruction_generator::generate<execute::rox_implicit_handler<false, true>, false, true>(instructions, OPCODE_BASE | OPCODE_ROX );
            x_instruction_generator::generate<execute::rox_implicit_handler<true, true>, false, true>(instructions, OPCODE_BASE | OPCODE_ROX | OPCODE_LEFT);
        }
        static void generate_roxx_ea(instructions_t& instructions) {
            unary_instruction_generator::generate<execute::rox_ea_handler<false, true>, word_t>(instructions, OPCODE_BASE | OPCODE_ROX_EA);
            unary_instruction_generator::generate<execute::rox_ea_handler<true, true>, word_t>(instructions, OPCODE_BASE | OPCODE_LEFT | OPCODE_ROX_EA);
        }
    };
}
#endif //SHIFT_ROTATION_GENERATOR_H

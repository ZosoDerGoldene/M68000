//
// Created by zoso on 11/18/24.
//

#include "generate/add_generator.h"
#include "generate/and_generator.h"
#include "generate/bcc_generator.h"
#include "generate/bcd_generator.h"
#include "generate/eor_generator.h"
#include "generate/move_generator.h"
#include "generate/or_generator.h"
#include "generate/shift_rotation_generator.h"
#include "generate/sub_generator.h""

#include "instructions.h"
namespace cpu::instructions {
    void instructions::generate_instructions(instructions_t &instructions) {
        generate::add_generator::generate_add(instructions);
        generate::add_generator::generate_adda(instructions);
        generate::add_generator::generate_addi(instructions);
        generate::add_generator::generate_addq(instructions);
        generate::add_generator::generate_addx(instructions);
        generate::and_generator::generate_and(instructions);
        generate::and_generator::generate_andi(instructions);
        generate::and_generator::generate_andi2ccr(instructions);
        generate::asx_generator::generate_asx(instructions);
        generate::bcc_generator::generate_bcc(instructions);
        generate::bcd_generator::generate_abcd(instructions);
        generate::bcd_generator::generate_sbcd(instructions);
        generate::bcd_generator::generate_pack(instructions);
        generate::bcd_generator::generate_unpk(instructions);
        generate::eor_generator::generate_eor(instructions);
        generate::eor_generator::generate_eori(instructions);
        generate::eor_generator::generate_eori2ccr(instructions);
        generate::lsx_generator::generate_lsx(instructions);
        generate::move_generator::generate_move(instructions);
        generate::move_generator::generate_movea(instructions);
        generate::move_generator::generate_moveq(instructions);
        generate::or_generator::generate_or(instructions);
        generate::or_generator::generate_ori(instructions);
        generate::or_generator::generate_ori2ccr(instructions);
        generate::rox_generator::generate_rox(instructions);
        generate::roxx_generator::generate_roxx(instructions);
        generate::sub_generator::generate_sub(instructions);
        generate::sub_generator::generate_suba(instructions);
        generate::sub_generator::generate_subi(instructions);
        generate::sub_generator::generate_subq(instructions);
        generate::sub_generator::generate_subx(instructions);
    }

}
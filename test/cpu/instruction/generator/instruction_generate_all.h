//
// Created by Benjamin Landorff on 6/24/21.
//

#ifndef INSTRUCTION_BUILDING_ALL_H
#define INSTRUCTION_BUILDING_ALL_H

#include "instruction_generator_common.h"
#include "instruction_generator_add_test.h"
#include "instruction_generator_and_test.h"
#include "instruction_generator_bcc_test.h"
#include "instruction_generator_bcd_test.h"
#include "instruction_generator_cmp_test.h"
#include "instruction_generator_eor_test.h"
#include "instruction_generator_lea_test.h"
#include "instruction_generator_move_test.h"
#include "instruction_generator_or_test.h"
#include "instruction_generator_shift_rotate_test.h"
#include "instruction_generator_sub_test.h"

constexpr counter_t total_count =
    abcd_count + add_count + adda_count + addi_count + addq_count + addx_count + and_count + andi_count +
    andi2ccr_count + asx_count + bcc_count + cmp_count + cmpa_count + cmpi_count +
        eor_count + eori_count + eori2ccr_count + lea_count + lsx_count +
    move_count + movea_count + moveq_count + pack_count + or_count + ori_count + ori2ccr_count + rox_count +
    roxx_count + sbcd_count + sub_count + suba_count + subi_count + subq_count + subx_count + unpk_count ;

#endif //INSTRUCTION_BUILDING_ALL_H

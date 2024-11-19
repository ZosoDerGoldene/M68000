//
// Created by Benjamin Landorff on 6/24/21.
//

#ifndef AMIGA_FEVER_INSTRUCTION_GENERATOR_SUB_TEST_H
#define AMIGA_FEVER_INSTRUCTION_GENERATOR_SUB_TEST_H

#include "instruction_generator_common.h"
#include "../../../../src/cpu/instructions/generate/sub_generator.h"
constexpr counter_t sub_count = 8*(2*(8*7+5)+(8*6+5))+(3*8*(8*5+2));
constexpr counter_t subi_count = 3*(8*6+2);
constexpr counter_t subq_count = 2*(8*(7*8+2))+(8*(6*8+2));
constexpr counter_t suba_count = 2*(8*(7*8+5));
constexpr counter_t subx_count = 2*3*(8*8);
#endif //AMIGA_FEVER_INSTRUCTION_GENERATOR_SUB_TEST_H

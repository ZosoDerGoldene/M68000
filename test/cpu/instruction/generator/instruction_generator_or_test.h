//
// Created by Benjamin Landorff on 6/24/21.
//

#ifndef INSTRUCTION_GENERATOR_OR_TEST_H
#define INSTRUCTION_GENERATOR_OR_TEST_H

#include "instruction_generator_common.h"
#include "../../../../src/cpu/instructions/generate/or_generator.h"

constexpr counter_t or_count = 3*(8*(6*8+5))+3*(8*(5*8+2));
constexpr counter_t ori_count = 3*(8*6+2);
constexpr counter_t ori2ccr_count = 1;

#endif //INSTRUCTION_GENERATOR_OR_TEST_H

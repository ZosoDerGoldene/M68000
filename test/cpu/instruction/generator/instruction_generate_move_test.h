//
// Created by Benjamin Landorff on 6/24/21.
//

#ifndef INSTRUCTION_GENERATOR_MOVE_TEST_H
#define INSTRUCTION_GENERATOR_MOVE_TEST_H

#include "instruction_generator_common.h"
#include "../../../../src/cpu/instructions/generate/move_generator.h"

constexpr counter_t move_count = (2*(8*7+5)+(8*6+5))*(8*6+2);
constexpr counter_t movea_count = 2*8*(8*7+5);
constexpr counter_t moveq_count = 8*256;

#endif //INSTRUCTION_GENERATOR_MOVE_TEST_H

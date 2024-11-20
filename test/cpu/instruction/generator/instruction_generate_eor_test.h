//
// Created by Benjamin Landorff on 6/24/21.
//

#ifndef INSTRUCTION_GENERATOR_EOR_TEST_H
#define INSTRUCTION_GENERATOR_EOR_TEST_H

#include "instruction_generator_common.h"
#include "../../../../src/cpu/instructions/generate/eor_generator.h"

constexpr counter_t eor_count = 3*8*(6*8+2);
constexpr counter_t eori_count = 3*(6*8+2);
constexpr counter_t eori2ccr_count = 1;

#endif //INSTRUCTION_GENERATOR_EOR_TEST_H

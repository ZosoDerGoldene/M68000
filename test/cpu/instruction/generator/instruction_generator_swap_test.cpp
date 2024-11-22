//
// Created by zoso on 11/22/24.
//

#include "instruction_generator_swap_test.h"

TEST_F(instruction_generator, generate_swap) {
    clean_instructions();
    const counter_t before = instructions_count();
    cpu::instructions::generate::swap_generator::generate_swap(_instructions);
    const counter_t after = instructions_count();
    ASSERT_EQ(after - before, swap_count);
}
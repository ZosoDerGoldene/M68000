//
// Created by zoso on 11/22/24.
//

#include "instruction_generator_not_test.h"

TEST_F(instruction_generator, generate_not) {
    clean_instructions();
    const counter_t before = instructions_count();
    cpu::instructions::generate::not_generator::generate_not(_instructions);
    const counter_t after = instructions_count();
    ASSERT_EQ(after - before, not_count);
}
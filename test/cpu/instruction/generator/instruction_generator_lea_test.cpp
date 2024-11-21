//
// Created by zoso on 11/21/24.
//

#include "instruction_generator_lea_test.h"

TEST_F(instruction_building, build_lea) {
    clean_instructions();
    const counter_t before = instructions_count();
    cpu::instructions::generate::lea_generator::generate_lea(_instructions);
    const counter_t after = instructions_count();
    ASSERT_EQ(after - before, lea_count);
}
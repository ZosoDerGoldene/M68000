//
// Created by zoso on 11/22/24.
//

#include "instruction_generator_clr_test.h"

TEST_F(instruction_building, generate_clr) {
    clean_instructions();
    const counter_t before = instructions_count();
    cpu::instructions::generate::clr_generator::generate_clr(_instructions);
    const counter_t after = instructions_count();
    ASSERT_EQ(after - before, clr_count);
}
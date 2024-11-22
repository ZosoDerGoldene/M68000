//
// Created by zoso on 11/22/24.
//

#include "instruction_generator_nop_test.h"

TEST_F(instruction_building, generate_nop) {
    clean_instructions();
    const counter_t before = instructions_count();
    cpu::instructions::generate::nop_generator::generate_nop(_instructions);
    const counter_t after = instructions_count();
    ASSERT_EQ(after - before, nop_count);
}
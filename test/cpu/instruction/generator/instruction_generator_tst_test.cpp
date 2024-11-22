//
// Created by zoso on 11/22/24.
//

#include "instruction_generator_tst_test.h"

TEST_F(instruction_generator, generate_tst) {
    clean_instructions();
    const counter_t before = instructions_count();
    cpu::instructions::generate::tst_generator::generate_tst(_instructions);
    const counter_t after = instructions_count();
    ASSERT_EQ(after - before, tst_count);
}
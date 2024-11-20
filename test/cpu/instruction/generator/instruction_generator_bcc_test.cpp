//
// Created by Benjamin Landorff on 6/24/21.
//

#include "instruction_generator_bcc_test.h"

TEST_F(instruction_building, build_bcc) {
    clean_instructions();
    const counter_t before = instructions_count();
    cpu::instructions::generate::bcc_generator::generate_bcc(_instructions);
    const counter_t after = instructions_count();
    ASSERT_EQ(after - before, bcc_count);
}
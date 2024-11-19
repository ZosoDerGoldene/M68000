//
// Created by Benjamin Landorff on 6/24/21.
//

#include "instruction_generator_or_test.h"

TEST_F(instruction_building, build_or) {
    clean_instructions();
    counter_t before = instructions_count();
    cpu::instructions::generate::or_generator::generate_or(_instructions);
    counter_t after = instructions_count();
    ASSERT_EQ(after - before, or_count);
}

TEST_F(instruction_building, build_ori) {
    clean_instructions();
    counter_t before = instructions_count();
    cpu::instructions::generate::or_generator::generate_ori(_instructions);
    counter_t after = instructions_count();
    ASSERT_EQ(after - before, ori_count);
}

TEST_F(instruction_building, build_ori2ccr) {
    clean_instructions();
    counter_t before = instructions_count();
    cpu::instructions::generate::or_generator::generate_ori2ccr(_instructions);
    counter_t after = instructions_count();
    ASSERT_EQ(after - before, ori2ccr_count);
}
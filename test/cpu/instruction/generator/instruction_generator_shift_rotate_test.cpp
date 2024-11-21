//
// Created by Benjamin Landorff on 6/24/21.
//

#include "instruction_generator_shift_rotate_test.h"

TEST_F(instruction_building, build_asx) {
    clean_instructions();
    counter_t before = instructions_count();
    cpu::instructions::generate::asx_generator::generate_asx(_instructions);
    counter_t after = instructions_count();
    ASSERT_EQ(after - before, asx_count);
}

TEST_F(instruction_building, build_lsx) {
    clean_instructions();
    counter_t before = instructions_count();
    cpu::instructions::generate::lsx_generator::generate_lsx(_instructions);
    counter_t after = instructions_count();
    ASSERT_EQ(after - before, lsx_count);
}

TEST_F(instruction_building, build_rox) {
    clean_instructions();
    counter_t before = instructions_count();
    cpu::instructions::generate::rox_generator::generate_rox(_instructions);
    counter_t after = instructions_count();
    ASSERT_EQ(after - before, rox_count);
}


TEST_F(instruction_building, build_roxx) {
    clean_instructions();
    counter_t before = instructions_count();
    cpu::instructions::generate::roxx_generator::generate_roxx(_instructions);
    counter_t after = instructions_count();
    ASSERT_EQ(after - before, roxx_count);
}


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

TEST_F(instruction_building, build_asx_dn) {
    clean_instructions();
    counter_t before = instructions_count();
    cpu::instructions::generate::asx_generator::generate_asx_dn(_instructions);
    counter_t after = instructions_count();
    ASSERT_EQ(after - before, asx_dn_count);
}

TEST_F(instruction_building, build_asx_implicit) {
    clean_instructions();
    counter_t before = instructions_count();
    cpu::instructions::generate::asx_generator::generate_asx_implicit(_instructions);
    counter_t after = instructions_count();
    ASSERT_EQ(after - before, asx_implicit_count);
}

TEST_F(instruction_building, build_asx_ea) {
    clean_instructions();
    counter_t before = instructions_count();
    cpu::instructions::generate::asx_generator::generate_asx_ea(_instructions);
    counter_t after = instructions_count();
    ASSERT_EQ(after - before, asx_ea_count);
}


TEST_F(instruction_building, build_lsx) {
    clean_instructions();
    counter_t before = instructions_count();
    cpu::instructions::generate::lsx_generator::generate_lsx(_instructions);
    counter_t after = instructions_count();
    ASSERT_EQ(after - before, lsx_count);
}

TEST_F(instruction_building, build_lsx_dn) {
    clean_instructions();
    counter_t before = instructions_count();
    cpu::instructions::generate::lsx_generator::generate_lsx_dn(_instructions);
    counter_t after = instructions_count();
    ASSERT_EQ(after - before, lsx_dn_count);
}

TEST_F(instruction_building, build_lsx_implicit) {
    clean_instructions();
    counter_t before = instructions_count();
    cpu::instructions::generate::lsx_generator::generate_lsx_implicit(_instructions);
    counter_t after = instructions_count();
    ASSERT_EQ(after - before, lsx_implicit_count);
}

TEST_F(instruction_building, build_lsx_ea) {
    clean_instructions();
    counter_t before = instructions_count();
    cpu::instructions::generate::lsx_generator::generate_lsx_ea(_instructions);
    counter_t after = instructions_count();
    ASSERT_EQ(after - before, lsx_ea_count);
}

TEST_F(instruction_building, build_rox) {
    clean_instructions();
    counter_t before = instructions_count();
    cpu::instructions::generate::rox_generator::generate_rox(_instructions);
    counter_t after = instructions_count();
    ASSERT_EQ(after - before, rox_count);
}

TEST_F(instruction_building, build_rox_dn) {
    clean_instructions();
    counter_t before = instructions_count();
    cpu::instructions::generate::rox_generator::generate_rox_dn(_instructions);
    counter_t after = instructions_count();
    ASSERT_EQ(after - before, rox_dn_count);
}

TEST_F(instruction_building, build_rox_implicit) {
    clean_instructions();
    counter_t before = instructions_count();
    cpu::instructions::generate::rox_generator::generate_rox_implicit(_instructions);
    counter_t after = instructions_count();
    ASSERT_EQ(after - before, rox_implicit_count);
}

TEST_F(instruction_building, build_rox_ea) {
    clean_instructions();
    counter_t before = instructions_count();
    cpu::instructions::generate::rox_generator::generate_rox_ea(_instructions);
    counter_t after = instructions_count();
    ASSERT_EQ(after - before, rox_ea_count);
}

TEST_F(instruction_building, build_roxx) {
    clean_instructions();
    counter_t before = instructions_count();
    cpu::instructions::generate::roxx_generator::generate_roxx(_instructions);
    counter_t after = instructions_count();
    ASSERT_EQ(after - before, roxx_count);
}

TEST_F(instruction_building, build_roxx_dn) {
    clean_instructions();
    counter_t before = instructions_count();
    cpu::instructions::generate::roxx_generator::generate_roxx_dn(_instructions);
    counter_t after = instructions_count();
    ASSERT_EQ(after - before, roxx_dn_count);
}

TEST_F(instruction_building, build_roxx_implicit) {
    clean_instructions();
    counter_t before = instructions_count();
    cpu::instructions::generate::roxx_generator::generate_roxx_implicit(_instructions);
    counter_t after = instructions_count();
    ASSERT_EQ(after - before, roxx_implicit_count);
}

TEST_F(instruction_building, build_roxx_ea) {
    clean_instructions();
    counter_t before = instructions_count();
    cpu::instructions::generate::roxx_generator::generate_roxx_ea(_instructions);
    counter_t after = instructions_count();
    ASSERT_EQ(after - before, roxx_ea_count);
}


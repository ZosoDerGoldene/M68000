//
// Created by Benjamin Landorff on 6/24/21.
//

#include "instruction_generator_bcd_test.h"

TEST_F(instruction_generator, build_abcd) {
    clean_instructions();
    counter_t before = instructions_count();
    cpu::instructions::generate::bcd_generator::generate_abcd(_instructions);
    counter_t after = instructions_count();
    ASSERT_EQ(after - before, abcd_count);
}

TEST_F(instruction_generator, build_pack) {
    clean_instructions();
    counter_t before = instructions_count();
    cpu::instructions::generate::bcd_generator::generate_pack(_instructions);
    counter_t after = instructions_count();
    ASSERT_EQ(after - before, pack_count);
}

TEST_F(instruction_generator, build_sbcd) {
    clean_instructions();
    counter_t before = instructions_count();
    cpu::instructions::generate::bcd_generator::generate_sbcd(_instructions);
    counter_t after = instructions_count();
    ASSERT_EQ(after - before, sbcd_count);
}

TEST_F(instruction_generator, build_unpk) {
    clean_instructions();
    counter_t before = instructions_count();
    cpu::instructions::generate::bcd_generator::generate_unpk(_instructions);
    counter_t after = instructions_count();
    ASSERT_EQ(after - before, unpk_count);
}

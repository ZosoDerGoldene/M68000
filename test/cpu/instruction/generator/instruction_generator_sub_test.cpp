//
// Created by Benjamin Landorff on 6/24/21.
//

#include "instruction_generator_sub_test.h"

TEST_F(instruction_generator, build_sub) {
    clean_instructions();
    counter_t before = instructions_count();
    cpu::instructions::generate::sub_generator::generate_sub(_instructions);
    counter_t after = instructions_count();
    ASSERT_EQ(after - before, sub_count);
}

TEST_F(instruction_generator, build_subi) {
    clean_instructions();
    counter_t before = instructions_count();
    cpu::instructions::generate::sub_generator::generate_subi(_instructions);
    counter_t after = instructions_count();
    ASSERT_EQ(after - before, subi_count);
}

TEST_F(instruction_generator, build_suba) {
    clean_instructions();
    counter_t before = instructions_count();
    cpu::instructions::generate::sub_generator::generate_suba(_instructions);
    counter_t after = instructions_count();
    ASSERT_EQ(after - before, suba_count);
}

TEST_F(instruction_generator, build_subx) {
    clean_instructions();
    counter_t before = instructions_count();
    cpu::instructions::generate::sub_generator::generate_subx(_instructions);
    counter_t after = instructions_count();
    ASSERT_EQ(after - before, subx_count);
}

TEST_F(instruction_generator, build_subq) {
    clean_instructions();
    counter_t before = instructions_count();
    cpu::instructions::generate::sub_generator::generate_subq(_instructions);
    counter_t after = instructions_count();
    ASSERT_EQ(after - before, subq_count);
}

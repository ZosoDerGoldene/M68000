//
// Created by Benjamin Landorff on 6/24/21.
//

#include "gtest/gtest.h"
#include "instruction_generator_and_test.h"
#include "../../../../src/cpu/instructions/generate/and_generator.h"

TEST_F(instruction_generator, build_and) {
    clean_instructions();
    counter_t before = instructions_count();
    cpu::instructions::generate::and_generator::generate_and(_instructions);
    counter_t after = instructions_count();
    ASSERT_EQ(after - before, and_count);
}

TEST_F(instruction_generator, build_andi) {
    clean_instructions();
    counter_t before = instructions_count();
    cpu::instructions::generate::and_generator::generate_andi(_instructions);
    counter_t after = instructions_count();
    ASSERT_EQ(after - before, andi_count);
}

TEST_F(instruction_generator, build_andi2ccr) {
    clean_instructions();
    counter_t before = instructions_count();
    cpu::instructions::generate::and_generator::generate_andi2ccr(_instructions);
    counter_t after = instructions_count();
    ASSERT_EQ(after - before, andi2ccr_count);
}

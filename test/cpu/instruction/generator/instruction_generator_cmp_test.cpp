//
// Created by zoso on 11/22/24.
//

#include "instruction_generator_cmp_test.h"

TEST_F(instruction_generator, generate_cmp) {
    clean_instructions();
    counter_t before = instructions_count();
    cpu::instructions::generate::cmp_generator::generate_cmp(_instructions);
    counter_t after = instructions_count();
    ASSERT_EQ(after - before, cmp_count);
}

TEST_F(instruction_generator, generate_cmpi) {
    clean_instructions();
    counter_t before = instructions_count();
    cpu::instructions::generate::cmp_generator::generate_cmpi(_instructions);
    counter_t after = instructions_count();
    ASSERT_EQ(after - before, cmpi_count);
}

TEST_F(instruction_generator, generate_cmpa) {
    clean_instructions();
    counter_t before = instructions_count();
    cpu::instructions::generate::cmp_generator::generate_cmpa(_instructions);
    counter_t after = instructions_count();
    ASSERT_EQ(after - before, cmpa_count);
}
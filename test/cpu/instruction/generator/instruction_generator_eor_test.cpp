//
// Created by Benjamin Landorff on 6/24/21.
//

#include "instruction_generator_eor_test.h"
TEST_F(instruction_generator, build_eor) {
    clean_instructions();
    counter_t before = instructions_count();
    cpu::instructions::generate::eor_generator::generate_eor(_instructions);
    counter_t after = instructions_count();
    ASSERT_EQ(after - before, eor_count);
}

TEST_F(instruction_generator, build_eori) {
    clean_instructions();
    counter_t before = instructions_count();
    cpu::instructions::generate::eor_generator::generate_eori(_instructions);
    counter_t after = instructions_count();
    ASSERT_EQ(after - before, eori_count);
}

TEST_F(instruction_generator, build_eori2ccr) {
    clean_instructions();
    counter_t before = instructions_count();
    cpu::instructions::generate::eor_generator::generate_eori2ccr(_instructions);
    counter_t after = instructions_count();
    ASSERT_EQ(after - before, eori2ccr_count);
}

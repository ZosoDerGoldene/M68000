//
// Created by Benjamin Landorff on 6/10/21.
//

#include "instruction_generator_add_test.h"

TEST_F(instruction_building, generate_add) {
    clean_instructions();
    const counter_t before = instructions_count();
    cpu::instructions::generate::add_generator::generate_add(_instructions);
    const counter_t after = instructions_count();
    ASSERT_EQ(after - before, add_count);
}

TEST_F(instruction_building, generate_adda) {
    clean_instructions();
    const counter_t before = instructions_count();
    cpu::instructions::generate::add_generator::generate_adda(_instructions);
    const counter_t after = instructions_count();
    ASSERT_EQ(after - before, adda_count);
}

TEST_F(instruction_building, generate_addi) {
    clean_instructions();
    const counter_t before = instructions_count();
    cpu::instructions::generate::add_generator::generate_addi(_instructions);
    const counter_t after = instructions_count();
    ASSERT_EQ(after - before, addi_count);
}

TEST_F(instruction_building, generate_addq) {
    clean_instructions();
    const counter_t before = instructions_count();
    cpu::instructions::generate::add_generator::generate_addq(_instructions);
    const counter_t after = instructions_count();
    ASSERT_EQ(after - before, addq_count);
}

TEST_F(instruction_building, generate_addx) {
    clean_instructions();
    const counter_t before = instructions_count();
    cpu::instructions::generate::add_generator::generate_addx(_instructions);
    const counter_t after = instructions_count();
    ASSERT_EQ(after - before, addx_count);
}


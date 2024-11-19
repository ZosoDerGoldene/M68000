//
// Created by Benjamin Landorff on 6/24/21.
//

#include "instruction_generate_move_test.h"

TEST_F(instruction_building, build_move) {
    clean_instructions();
    const counter_t before = instructions_count();
    cpu::instructions::generate::move_generator::generate_move(_instructions);
    const counter_t after = instructions_count();
    ASSERT_EQ(after - before, move_count);
}

TEST_F(instruction_building, build_movea) {
    clean_instructions();
    const counter_t before = instructions_count();
    cpu::instructions::generate::move_generator::generate_movea(_instructions);
    const counter_t after = instructions_count();
    ASSERT_EQ(after - before, movea_count);
}

TEST_F(instruction_building, build_moveq) {
    clean_instructions();
    const counter_t before = instructions_count();
    cpu::instructions::generate::move_generator::generate_moveq(_instructions);
    const counter_t after = instructions_count();
    ASSERT_EQ(after - before, moveq_count);
}

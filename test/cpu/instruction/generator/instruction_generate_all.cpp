//
// Created by Benjamin Landorff on 6/24/21.
//


#include "instruction_generate_all.h"
TEST_F(instruction_generator, build_all) {
    clean_instructions();
    const counter_t before = instructions_count();
    cpu::instructions::instructions::generate_instructions(_instructions);
    const counter_t after = instructions_count();
    ASSERT_EQ(after - before, total_count);
    std::cout << total_count << " " << static_cast<double>(total_count)*100.0/static_cast<double>(0x10000) << "%" << std::endl;
}

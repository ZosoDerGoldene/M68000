cmake_minimum_required(VERSION 3.19)
project(test)
set(CMAKE_CXX_STANDARD 20)
# 'lib' is the folder with Google Test sources
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
# 'test1.cpp test2.cpp' are source files with tests
add_executable(run_tests
        types_test.cpp
        mem/mem_bank_test.cpp
        cpu/status_register_test.cpp
        cpu/effective_addressing_mode_test.cpp
        cpu/instruction/generator/instruction_generator_add_test.cpp
        cpu/instruction/generator/instruction_generator_add_test.h
        ../src/memory/layout.cpp
        ../src/memory/memory.cpp
        ../src/cpu/instructions/instructions.h
        ../src/cpu/instructions/instructions.cpp
        cpu/instruction/generator/instruction_generator_common.h
        cpu/instruction/generator/instruction_generator_and_test.cpp
        cpu/instruction/generator/instruction_generator_and_test.h
        cpu/instruction/generator/instruction_generator_sub_test.cpp
        cpu/instruction/generator/instruction_generator_sub_test.h
        cpu/instruction/generator/instruction_generator_shift_rotate_test.h
        cpu/instruction/generator/instruction_generator_shift_rotate_test.cpp
        cpu/instruction/generator/instruction_generator_or_test.h
        cpu/instruction/generator/instruction_generator_or_test.cpp
        cpu/instruction/generator/instruction_generator_move_test.cpp
        cpu/instruction/generator/instruction_generator_move_test.h
        cpu/instruction/generator/instruction_generator_bcc_test.h
        cpu/instruction/generator/instruction_generator_bcc_test.cpp
        cpu/instruction/generator/instruction_generator_eor_test.h
        cpu/instruction/generator/instruction_generator_eor_test.cpp
        cpu/instruction/generator/instruction_generator_bcd_test.h
        cpu/instruction/generator/instruction_generator_bcd_test.cpp
        cpu/instruction/generator/instruction_generate_all.cpp
        cpu/instruction/generator/instruction_generate_all.h
        cpu/instruction/generator/instruction_generator_lea_test.cpp
        cpu/instruction/generator/instruction_generator_lea_test.h
        cpu/instruction/generator/instruction_generator_cmp_test.cpp
        cpu/instruction/generator/instruction_generator_cmp_test.h
        cpu/instruction/generator/instruction_generator_nop_test.cpp
        cpu/instruction/generator/instruction_generator_nop_test.h
        cpu/instruction/generator/instruction_generator_clr_test.cpp
        cpu/instruction/generator/instruction_generator_clr_test.h

)
target_link_libraries(run_tests gtest gtest_main)
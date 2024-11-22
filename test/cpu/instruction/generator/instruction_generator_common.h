//
// Created by Benjamin Landorff on 6/24/21.
//

#ifndef INSTRUCTION_GENERATOR_COMMON_H
#define INSTRUCTION_GENERATOR_COMMON_H

#include "gtest/gtest.h"
#include "../../../../src/memory/memory.h"
#include "../../../../src/memory/layout.h"
#include "../../../../src/cpu/cpu.h"
#include "../../../../src/cpu/instructions/instructions.h"

using counter_t = unsigned long int;
struct instruction_generator : public ::testing::Test {
    instruction_generator() : _cpu(_mem), _layout(8, 0, 0) {
        _mem.reserve(_layout);
    }
protected:
    memory::memory _mem;
    cpu::cpu _cpu;
    memory::amiga_500::a500_layout _layout;
    cpu::instructions::instructions_t _instructions;
    counter_t instructions_count() {
        counter_t count = 0;
        for (counter_t i = 0; i < 0x10000; i++) {
            if (_instructions[i] != nullptr) count++;
        }
        return count;
    }
    void clean_instructions() {
        for (counter_t i = 0; i < 0x10000; i++) {
            _instructions[i] = nullptr;
        }
    }
};

#endif //INSTRUCTION_GENERATOR_COMMON_H

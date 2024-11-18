//
// Created by zoso on 11/18/24.
//

#include "generate/add_generator.h"

#include "instructions.h"
namespace cpu::instructions {
    void instructions::generate_instructions(instructions_t &instructions) {
        generate::add_generator::generate_add(instructions);
    }

}
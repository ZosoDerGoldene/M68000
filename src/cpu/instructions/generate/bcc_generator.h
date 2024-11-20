//
// Created by zoso on 11/19/24.
//

#ifndef BCC_GENERATOR_H
#define BCC_GENERATOR_H

#include "../instructions.h"
namespace cpu::instructions::generate {
    constexpr opcode_t OPCODE_BCC = 0b0110000000000000;

    struct bcc_generator {
        static inline void generate_bcc(instructions_t& instructions) {
            // TODO: (Maybe) Handle immediate long displacements (68020++), identified by implicit $ff displacements
            // Condition = 0 and displacement = 0 means bsr.w
            instructions[OPCODE_BCC] =
                static_cast<instruction_fn>(instructions::execution_wrapper<execute::bsr_handler, word_t, ea::imm>);
            for (opcode_t i = 1; i < 0x1000; i++) {
                const opcode_t opcode = OPCODE_BCC | i;
                // Bits 11-8 encode the condition; 0 means true, so this makes it bsr
                if ((opcode & 0xf00) == 0) {
                    instructions[opcode] = static_cast<instruction_fn>(instructions::execution_wrapper<execute::bsr_handler>);
                } else {
                    instructions[opcode] = static_cast<instruction_fn>(instructions::execution_wrapper<execute::bcc_handler>);
                }
            }
            // For performance reasons, we didn't look for 0 displacements (meaning word size) in the first loop
            // rather, we're overwriting the affected ones in the second loop
            // condition 0 (true, or bsr) has already been handled as the first separate case
            for (opcode_t i = 0x100; i < 0x1000; i+= 0x100) {
                const opcode_t opcode = OPCODE_BCC | i;
                instructions[opcode] = static_cast<instruction_fn>(instructions::execution_wrapper<execute::bcc_handler, word_t, ea::imm>);
            }
        }
    };
}
#endif //BCC_GENERATOR_H

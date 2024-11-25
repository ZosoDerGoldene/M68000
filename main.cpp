//
// Created by zoso on 11/18/24.
//

// If a "the_rom" Amiga rom file is present, this should be defined
#define WITH_ROM


#include <iostream>

#include "src/cpu/cpu.h"
#include "src/memory/memory.h"
#include "src/cpu/instructions/instructions.h"

[[noreturn]] int main() {
  std::cout << "Hello Amiga!" << std::endl;
  memory::memory mem;
  memory::amiga_500::a500_layout l(8,0,0);
  mem.reserve(l);
  cpu::instructions::instructions_t instructions;
  cpu::instructions::instructions::generate_instructions(instructions);
  cpu::cpu m68000(mem);
  m68000.reset();
  while (true) {
    if (m68000.pc().get_pc() == 16515298) {
        std::cout << "Gotcha!" << std::endl;
    }
    const opcode_t next_opcode = m68000.pc().get_next_opcode();
    instructions[next_opcode](m68000); // TODO: This currently crashes, as there is not function to handel lea instructions
  }
}

cmake_minimum_required(VERSION 3.29)
project(M68000)

set(CMAKE_CXX_STANDARD 20)


add_executable(M68000
    main.cpp
        src/types.h
        src/cpu/instructions/execute/add_handler.h
        src/cpu/cpu.cpp
        src/cpu/cpu.h
        src/cpu/registers/program_counter.h
        src/memory/memory.cpp
        src/memory/memory.h
        src/memory/memory_bank.h
        src/memory/layout.cpp
        src/memory/layout.h
        src/cpu/registers/registers.h
        src/cpu/registers/status_register.h
        src/cpu/instructions/generate/add_generator.h
        src/cpu/instructions/instructions.cpp
        src/cpu/instructions/instructions.h
        src/cpu/instructions/execute/execution_handlers.h
        src/cpu/instructions/generate/binary_instruction_generator.h
        src/cpu/instructions/generate/x_instruction_generator.h
        src/cpu/instructions/generate/unary_instruction_generator.h
        src/cpu/instructions/execute/and_handler.h
        src/cpu/instructions/generate/and_generator.h
        src/cpu/instructions/execute/sub_handler.h
        src/cpu/instructions/generate/sub_generator.h
        src/cpu/instructions/generate/shift_rotation_generator.h
        src/cpu/instructions/execute/asx_handler.h
        src/cpu/instructions/execute/lsx_handler.h
        src/cpu/instructions/execute/rox_handler.h
        src/cpu/instructions/execute/or_handler.h
        src/cpu/instructions/generate/or_generator.h
        src/cpu/instructions/execute/move_handler.h
        src/cpu/instructions/generate/move_generator.h
        src/cpu/instructions/execute/bcc_handler.h
        src/cpu/instructions/generate/bcc_generator.h
        src/cpu/instructions/execute/eor_handler.h
        src/cpu/effective_address.h
        src/cpu/instructions/generate/eor_generator.h
        src/cpu/instructions/execute/bcd_handler.h
        src/cpu/instructions/generate/bcd_generator.h
        src/cpu/instructions/execute/lea_handler.h
        src/cpu/instructions/generate/lea_generator.h
        src/cpu/instructions/generate/nop_generator.h
        src/cpu/instructions/execute/nop_handler.h
        src/cpu/instructions/execute/clr_handler.h
        src/cpu/instructions/generate/clr_generator.h
        src/cpu/instructions/execute/swap_handler.h
        src/cpu/instructions/generate/swap_generator.h
        src/cpu/instructions/execute/tst_handler.h
        src/cpu/instructions/generate/tst_generator.h
        src/cpu/instructions/execute/not_handler.h
        src/cpu/instructions/generate/not_generator.h
)

add_subdirectory(test)

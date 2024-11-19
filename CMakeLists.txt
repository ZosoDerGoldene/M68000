cmake_minimum_required(VERSION 3.29)
project(M68000)

set(CMAKE_CXX_STANDARD 23)


add_executable(M68000
    main.cpp
        src/types.h
        src/cpu/execution_properties.h
        src/cpu/instructions/execute/add_handler.h
        src/cpu/ea/effective_address.h
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
        src/cpu/instructions/generate/x_instrcution_generator.h
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
)

add_subdirectory(test)

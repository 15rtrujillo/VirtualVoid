cmake_minimum_required(VERSION 3.10)
project(VirtualVoid)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(vv-asm
    "src/asm/main.cpp"
    "src/asm/assembler.cpp"
    "include/asm/assembler.h"
    "include/core/instruction_set.h"
)

add_executable(vv-vm
    "src/vm/main.cpp"
    "src/vm/virtual_machine.cpp"
    "include/vm/virtual_machine.h"
    "include/core/instruction_set.h"
)

target_include_directories(vv-asm PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(vv-vm PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

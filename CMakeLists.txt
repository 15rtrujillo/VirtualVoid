cmake_minimum_required(VERSION 3.10)
project(VirtualVoid)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(vv-asm
    "src/asm/main.cpp"
    "src/asm/Assembler.cpp"
)

add_executable(vv-vm
    "src/vm/main.cpp"
    "src/vm/VirtualMachine.cpp"
)

target_include_directories(vv-asm PRIVATE include)
target_include_directories(vv-vm PRIVATE include)

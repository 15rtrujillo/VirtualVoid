cmake_minimum_required(VERSION 3.10)
project(SpectreConsole)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(asm 
    "src/asm/main.cpp"
    "src/asm/Assembler.cpp"
)

add_executable(vm 
    "src/vm/main.cpp"
    "src/vm/VirtualMachine.cpp"
)

target_include_directories(asm PRIVATE include)
target_include_directories(vm PRIVATE include)

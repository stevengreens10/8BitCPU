cmake_minimum_required(VERSION 3.22)
project(CPU)

set(CMAKE_CXX_STANDARD 20)

add_executable(GenInstructionDecoder GenInstructionDecoder.cpp CPU.h Instruction.h)
add_executable(Assembler Assembler.cpp CPU.h Instruction.h)
add_executable(GenTextLCD GenTextLCD.cpp)

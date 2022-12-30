#ifndef CPU_INSTRUCTION_H
#define CPU_INSTRUCTION_H


#include <string>

struct Instruction {
    std::string mnemonic;
    // 4 bytes:
    // <1st Control Signals Stage 2> | <2nd Control Signals Stage 2> | <1st Control Signals Stage 1> | <2nd Control Signals Stage 1>
    unsigned int controlFlags;
};


#endif //CPU_INSTRUCTION_H

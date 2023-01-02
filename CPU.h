#ifndef CPU_CPU_H
#define CPU_CPU_H

#include <vector>
#include "Instruction.h"

const unsigned long long STAGE2 = 16;
const unsigned long long STAGE3 = 32;
const unsigned long long COND = STAGE3 + 16;

// COND RAM
const unsigned long long ONNOTZF = (1ULL << (COND + 3));
const unsigned long long ONZF = (1ULL << (COND + 2));
const unsigned long long ONNOTCF = (1ULL << (COND + 1));
const unsigned long long ONCF = (1ULL << COND);

// ROM 1
const unsigned long long PCIN = (1 << 15);
const unsigned long long PCOUT = (1 << 14);
const unsigned long long PCINC = (1 << 13);
const unsigned long long MEMIN = (1 << 12);
const unsigned long long MEMOUT = (1 << 11);
const unsigned long long MEMWRITE = (1 << 10);
const unsigned long long AIN = (1 << 9);
const unsigned long long AOUT = (1 << 8);

// ROM 2
const unsigned long long BIN = (1 << 7);
const unsigned long long BOUT = (1 << 6);
const unsigned long long ALUOUT = (1 << 5);
const unsigned long long SUB = (1 << 4);
const unsigned long long FLAGS = (1 << 3);
const unsigned long long OUTIN = (1 << 2);
const unsigned long long TXT = (1 << 1);
const unsigned long long HLT = (1 << 0);


class CPU {

public:
    static inline std::vector<Instruction> instructions{
            {"MOV A, <imm8>", PCOUT | MEMIN |
                              ((MEMOUT | AIN | PCINC) << STAGE2)},
            {"MOV B, <imm8>", PCOUT | MEMIN |
                              ((MEMOUT | BIN | PCINC) << STAGE2)},
            {"MOV A, B",      BOUT | AIN},
            {"MOV B, A",      AOUT | BIN},
            {"STA <imm8>",    PCOUT | MEMIN |
                              ((MEMOUT | MEMIN | PCINC) << STAGE2) |
                              ((AOUT | MEMWRITE) << STAGE3)},
            {"STB <imm8>",    PCOUT | MEMIN |
                              ((MEMOUT | MEMIN | PCINC) << STAGE2) |
                              ((BOUT | MEMWRITE) << STAGE3)},
            {"LDA B",         BOUT | MEMIN |
                              ((MEMOUT | AIN) << STAGE2)},
            {"LDB A",         AOUT | MEMIN |
                              ((MEMOUT | BIN) << STAGE2)},
            {"LDA <imm8>",    PCOUT | MEMIN |
                              ((MEMOUT | MEMIN | PCINC) << STAGE2) |
                              ((AIN | MEMOUT) << STAGE3)},
            {"LDB <imm8>",    PCOUT | MEMIN |
                              ((MEMOUT | MEMIN | PCINC) << STAGE2) |
                              ((BIN | MEMOUT) << STAGE3)},
            {"OUT",           AOUT | OUTIN},
            {"OUTTXT",        AOUT | OUTIN | TXT},
            {"ADD",           ALUOUT | AIN},
            {"SUB",           ALUOUT | AIN | SUB},
            {"CMP",           SUB | FLAGS},
            {"JMP <imm8>",    static_cast<unsigned long long>(PCOUT | MEMIN |
                                                              ((MEMOUT | PCIN) << STAGE2))},
            {"JE <imm8>",     static_cast<unsigned long long>(ONZF | PCOUT | MEMIN |
                                                              ((PCINC) << STAGE2) |
                                                              ((MEMOUT | PCIN) << STAGE3))},
            {"JC <imm8>",     static_cast<unsigned long long>(ONCF | PCOUT | MEMIN |
                                                              ((PCINC) << STAGE2) |
                                                              ((MEMOUT | PCIN) << STAGE3))},
            {"JG <imm8>",     static_cast<unsigned long long>(ONNOTCF | ONNOTZF | PCOUT | MEMIN |
                                                              ((PCINC) << STAGE2) |
                                                              ((MEMOUT | PCIN) << STAGE3))},
            {"HLT",           HLT},
    };
};


#endif //CPU_CPU_H

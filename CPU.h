#ifndef CPU_CPU_H
#define CPU_CPU_H

#include <vector>
#include "Instruction.h"

// ROM 1
const int PCIN = (1 << 15);
const int PCOUT = (1 << 14);
const int PCINC = (1 << 13);
const int MEMIN = (1 << 12);
const int MEMOUT = (1 << 11);
const int MEMWRITE = (1 << 10);
const int AIN = (1 << 9);
const int AOUT = (1 << 8);

// ROM 2
const int BIN = (1 << 7);
const int BOUT = (1 << 6);
const int ALUOUT = (1 << 5);
const int SUB = (1 << 4);
const int FLAGS = (1 << 3);
const int OUTIN = (1 << 2);
const int TXT = (1 << 1);

const int STAGE2 = 16;

class CPU {

public:
    static inline std::vector<Instruction> instructions{
            {"MOV MAR, A",      MEMIN | AOUT},
            {"MOV MAR, B",      MEMIN | BOUT},
            {"MOV MAR, <imm8>", PCOUT | MEMIN |
                                ((MEMOUT | MEMIN | PCINC) << STAGE2)},
            {"MOV A, <imm8>",   PCOUT | MEMIN |
                                ((MEMOUT | AIN | PCINC) << STAGE2)},
            {"MOV B, <imm8>",   PCOUT | MEMIN |
                                ((MEMOUT | BIN | PCINC) << STAGE2)},
            {"STA",             AOUT | MEMWRITE},
            {"STB",             BOUT | MEMWRITE},
            {"LDA",             MEMOUT | AIN},
            {"LDB",             MEMOUT | BIN},
            {"OUT",             AOUT | OUTIN},
            {"OUTTXT",          AOUT | OUTIN | TXT},
            {"ADD",             ALUOUT | AIN},
            {"SUB",             ALUOUT | AIN | SUB},
            {"CMP",             SUB | FLAGS},
            {"JMP <imm8>",      static_cast<unsigned int>(PCOUT | MEMIN |
                                                          ((MEMOUT | PCIN) << STAGE2))},
    };
};


#endif //CPU_CPU_H

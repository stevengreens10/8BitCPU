#include <iostream>
#include <bitset>
#include <sstream>
#include <string>
#include "CPU.h"

int main() {
  std::cout << "Hello, GenInstructionDecoder!" << std::endl;
  std::stringstream rom1("");
  std::stringstream rom2("");
  for(unsigned char idx = 0; idx < CPU::instructions.size(); idx++) {
    auto inst = CPU::instructions[idx];
    unsigned short stage1Addr = (1 << 8) + idx;
    unsigned short stage2Addr = (1 << 9) + idx;
    
    std::cout << inst.mnemonic << std::endl;
    const std::bitset<8> &stage1Byte1 = std::bitset<8>(inst.controlFlags & 0xFF);
    const std::bitset<8> &stage1Byte2 = std::bitset<8>((inst.controlFlags & 0xFF00) >> 8);
    const std::bitset<8> &stage2Byte1 = std::bitset<8>((inst.controlFlags & 0xFF0000) >> 16);
    const std::bitset<8> &stage2Byte2 = std::bitset<8>((inst.controlFlags & 0xFF000000) >> 24);
    std::cout << "\tStage 1 Byte 1: (Addr = " << stage1Addr << ")" << stage1Byte1 << std::endl;
    std::cout << "\tStage 1 Byte 2: (Addr = " << stage1Addr << ")" << stage1Byte2 << std::endl;
    std::cout << "\tStage 2 Byte 1: (Addr = " << stage2Addr << ")" << stage2Byte1 << std::endl;
    std::cout << "\tStage 2 Byte 2: (Addr = " << stage2Addr << ")" << stage2Byte2 << std::endl;

    rom1 << stage1Addr << ": " << stage1Byte2.to_ulong() << std::endl;
    rom1 << stage2Addr << ": " << stage2Byte2.to_ulong() << std::endl;

    rom2 << stage1Addr << ": " << stage1Byte1.to_ulong() << std::endl;
    rom2 << stage2Addr << ": " << stage2Byte1.to_ulong() << std::endl;
  }

  std::cout << "ROM 1" << std::endl << "-----------------" << std::endl;
  std::cout << rom1.str();

  std::cout << std::endl;
  std::cout << "ROM 2" << std::endl << "-----------------" << std::endl;
  std::cout << rom2.str();
  return 0;
}

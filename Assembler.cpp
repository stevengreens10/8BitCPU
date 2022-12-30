#include <iostream>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <regex>
#include "CPU.h"

void processLine(std::string line);

void processData(std::string line);

void processCode(std::string line);

void processImmediate(std::string imm);

std::unordered_map<std::string, unsigned char> dataAddrs;
std::unordered_map<std::string, unsigned char> labelAddrs;
// Save room for "JMP, <codeBegin>"
int currentAddr = 2;
unsigned char codeBeginAddr = 0;
std::vector<unsigned char> bytes(256);

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage ./Assembler.exe <assembly file>";
    return 1;
  }

  std::string assemblyFile(argv[1]);
  std::cout << "Opening " << assemblyFile << std::endl;

  std::ifstream f;
  f.open(assemblyFile);
  if (!f.is_open()) {
    std::cerr << "Could not open " << assemblyFile << std::endl;
    return 1;
  }

  for (std::string line; std::getline(f, line);) {
    processLine(line);
  }

  for (auto it = dataAddrs.begin(); it != dataAddrs.end(); it++) {
    std::cout << "Variable " << it->first << " is at address: " << (int) it->second << std::endl;
  }

  std::cout << std::endl;

  for (auto it = labelAddrs.begin(); it != labelAddrs.end(); it++) {
    std::cout << "Label " << it->first << " is at address: " << (int) it->second << std::endl;
  }

  // JMP to beginning of code
  bytes[0] = 14;
  bytes[1] = codeBeginAddr;

  std::cout << "0: ";
  for (const auto &byte: bytes) {
    std::cout << (unsigned int) byte << " ";
  }
  std::cout << std::endl;

  return 0;
}

bool inDataSection = false;
bool inCodeSection = false;

void processLine(std::string line) {
  // Handle no-op first (only whitespace or comment)
  std::regex noopRegex("^\\s*(;.*$|\\s*$)", std::regex_constants::icase);
  if (std::regex_search(line, noopRegex)) {
    return;
  }

  // Handle .data or .code section headers
  std::regex dataRegex("^\\s*.data\\s*$", std::regex_constants::icase);
  if (std::regex_search(line, dataRegex)) {
    inDataSection = true;
    inCodeSection = false;
    return;
  }
  std::regex codeRegex("^\\s*.code\\s*$", std::regex_constants::icase);
  if (std::regex_search(line, codeRegex)) {
    inDataSection = false;
    inCodeSection = true;
    return;
  }

  if (inDataSection) {
    processData(line);
    return;
  }

  if (inCodeSection) {
    processCode(line);
    return;
  }

  std::cerr << "Unknown instruction out of .data or .code sections: " << line << std::endl;
  exit(1);
}

void processData(std::string line) {
  std::regex dataRegex(R"(^\s*(\w+)\s+=\s+(.*)\s*$)", std::regex_constants::icase);
  std::smatch m;
  if (std::regex_match(line, m, dataRegex)) {
    auto varName = m[1].str();
    auto value = m[2].str();
    std::cout << "VarName: " << varName << std::endl;
    dataAddrs[varName] = currentAddr;

    // Check if value is a string
    std::regex stringRegex("\"(.+)\"", std::regex_constants::icase);
    // Check if value is a decimal number
    std::regex decimalRegex("(\\d+)", std::regex_constants::icase);
    // Check if value is a hex number
    std::regex hexRegex("0x([0-9a-f]+)", std::regex_constants::icase);

    if (std::regex_match(value, m, stringRegex)) {
      auto text = m[1].str();
      std::cout << "Value (STR) = " << text << std::endl;

      for (const auto &ch: text) {
        bytes[currentAddr] = ch;
        currentAddr++;
      }

    } else if (std::regex_match(value, m, decimalRegex)) {
      unsigned int val = std::stoi(m[1].str());
      std::cout << "Value (DEC) = " << val << std::endl;

      if (val >= 0 && val <= 255) {
        bytes[currentAddr] = (unsigned char) val;
        currentAddr++;
      } else {
        std::cerr << "The decimal value for " << varName << " must be between 0 and 255";
        exit(1);
      }

    } else if (std::regex_match(value, m, hexRegex)) {
      unsigned int val;
      std::stringstream ss;
      ss << std::hex << m[1].str();
      ss >> val;

      std::cout << "Value (HEX) = 0x" << m[1] << std::endl;

      if (val >= 0 && val <= 255) {
        bytes[currentAddr] = (unsigned char) val;
        currentAddr++;
      } else {
        std::cerr << "The hex value for " << varName << " must be between 0x00 and 0xFF";
        exit(1);
      }
    } else {
      std::cerr << "Unknown data value for variable " << varName << ": " << value << std::endl;
      exit(1);
    }

    return;
  }

  std::cerr << "Unknown data declaration: " << line << std::endl;
  exit(1);
}

void processCode(std::string line) {
  if(codeBeginAddr == 0)
    codeBeginAddr = currentAddr;
  std::regex labelRegex(R"(^\s*:(\w+)\s*$)", std::regex_constants::icase);
  std::smatch m;
  if (std::regex_match(line, m, labelRegex)) {
    std::cout << "Label: " << m[1] << std::endl;
    labelAddrs[m[1].str()] = currentAddr;
    return;
  }

  for (int i = 0; i < CPU::instructions.size(); i++) {
    auto inst = CPU::instructions[i];
    bool hasImmediate = false;
    std::string mnemonicCopy(inst.mnemonic);
    auto pos = mnemonicCopy.find("<imm8>");
    if (pos != std::string::npos) {
      mnemonicCopy.replace(pos, 6, "(.*)");
      hasImmediate = true;
    }

    std::regex instRegex("^\\s*" + mnemonicCopy + "\\s*$", std::regex_constants::icase);
    if (std::regex_match(line, m, instRegex)) {
      std::cout << "Matched instruction: " << inst.mnemonic << std::endl;
      bytes[currentAddr] = i;
      currentAddr++;
      if (hasImmediate) {
        processImmediate(m[1].str());
      }
      return;
    }
  }

  std::cerr << "Code line did not match any valid instructions: " << line << std::endl;
  exit(1);
}

void processImmediate(std::string imm) {
  std::smatch m;
  // Handle variable address
  std::regex varRegex(R"(^\s*&(\w+)\s*$)", std::regex_constants::icase);
  if (std::regex_match(imm, m, varRegex)) {
    auto varName = m[1].str();

    if (!dataAddrs.contains(varName)) {
      std::cerr << "Referenced variable: " << varName << " has not been declared";
      exit(1);
    }

    unsigned char addr = dataAddrs[varName];
    std::cout << "Immediate address value of variable " << varName << " is " << (int) addr << std::endl;
    bytes[currentAddr] = addr;
    currentAddr++;
    return;
  }

  // Handle decimal value
  std::regex decimalRegex(R"(^\s*(\d+)\s*$)", std::regex_constants::icase);
  if (std::regex_match(imm, m, decimalRegex)) {
    unsigned int val = std::stoi(m[0].str());
    std::cout << "Immediate decimal value = " << val << std::endl;

    if (val >= 0 && val <= 255) {
      bytes[currentAddr] = (unsigned char) val;
      currentAddr++;
    } else {
      std::cerr << "The decimal immediate value must be between 0 and 255";
      exit(1);
    }
    return;
  }

  // Handle hex value
  std::regex hexRegex("^\\s*0x([0-9a-f]+)\\s*$", std::regex_constants::icase);
  if (std::regex_match(imm, m, hexRegex)) {
    unsigned int val;
    std::stringstream ss;
    ss << std::hex << m[1].str();
    ss >> val;

    std::cout << "Immediate hex value = 0x" << m[1] << std::endl;

    if (val >= 0 && val <= 255) {
      bytes[currentAddr] = (unsigned char) val;
      currentAddr++;
    } else {
      std::cerr << "The hex value must be between 0x00 and 0xFF";
      exit(1);
    }
    return;
  }
  // Handle char value
  std::regex charRegex(R"(^\s*'(\w| )'\s*$)", std::regex_constants::icase);
  if (std::regex_match(imm, m, charRegex)) {
    char val = m[1].str()[0];

    std::cout << "Immediate char value = '" << val << "'" << std::endl;

    bytes[currentAddr] = val;
    currentAddr++;

    return;
  }

  // Handle label address
  std::regex labelRegex(R"(^\s*(\w+)\s*$)", std::regex_constants::icase);
  if (std::regex_match(imm, m, labelRegex)) {
    auto labelName = m[1].str();

    if (!labelAddrs.contains(labelName)) {
      std::cerr << "Referenced label: " << labelName << " has not been declared";
      exit(1);
    }

    unsigned char addr = labelAddrs[labelName];
    std::cout << "Immediate address value of label " << labelName << " is " << (int) addr << std::endl;
    bytes[currentAddr] = addr;
    currentAddr++;
    return;
  }

  std::cerr << "Could not process immediate value: " << imm << std::endl;
  exit(1);

}
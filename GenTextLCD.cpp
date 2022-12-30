#include <unordered_map>
#include <iostream>

std::unordered_map<unsigned char, unsigned short> lcdSegments;

unsigned short A = (1 << 0);
unsigned short B = (1 << 1);
unsigned short C = (1 << 2);
unsigned short D = (1 << 3);
unsigned short E = (1 << 4);
unsigned short F = (1 << 5);
unsigned short G = (1 << 6);
unsigned short H = (1 << 7);
unsigned short I = (1 << 8);
unsigned short J = (1 << 9);
unsigned short K = (1 << 10);
unsigned short L = (1 << 11);
unsigned short M = (1 << 12);
unsigned short N = (1 << 13);

int main() {
  lcdSegments['H'] = B | C | E | F | J | N;
  lcdSegments['E'] = A | D | E | F | J | N;
  lcdSegments['L'] = D | E | F;
  lcdSegments['O'] = A | B | C | D | E | F;

  lcdSegments[' '] = 0;

  lcdSegments['W'] = B | C | E | F | K | M;
  lcdSegments['R'] = A | E | F | I | K | N;
  lcdSegments['D'] = E | F | G | M;

  for(auto it = lcdSegments.begin(); it != lcdSegments.end(); it++) {
    std::cout << ((int) it->first) << ": " << (int) it->second << std::endl;
  }

}
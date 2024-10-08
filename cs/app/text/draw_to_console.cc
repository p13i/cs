#include <iostream>

#include "cs/app/text/fonts/mono.hh"

using ::cs::app::text::fonts::mono;

int main() {
  // Print each character and it's associated mono font
  // drawn from an array
  for (uint i = 0; i < sizeof(mono); i++) {
    std::cout << (char)(i + 'A') << " (index=" << i << "):"
              << "\n";
    for (int y = 0; y < 8; y++) {
      for (int x = 0; x < 8; x++) {
        int flag = mono[i][y][x];
        if (flag) {
          std::cout << "o";
        } else {
          std::cout << " ";
        }
        std::cout << " ";
      }
      std::cout << "\n";
    }
    std::cout << "\n";
  }
}

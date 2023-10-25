#include <iostream>

#include "cs/app/text/fonts/mono.hh"

using ::cs::app::text::fonts::mono;

int main() {
  // Print each character and it's associated mono font
  // drawn from an array
  for (int i = 0; i < 1; i++) {
    std::cout << (char)(i + 97) << " (index=" << i
              << "):" << std::endl;
    for (int y = 0; y < 8; y++) {
      for (int x = 0; x < 8; x++) {
        int flag = mono[i][y][x];
        if (flag) {
          std::cout << ".";
        } else {
          std::cout << " ";
        }
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }
}

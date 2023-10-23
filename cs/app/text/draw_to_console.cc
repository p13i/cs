#include <iostream>

#include "cs/app/text/fonts/mono.hh"

using ::cs::app::text::fonts::mono;

int main() {
    // Print each character and it's associated mono font drawn from an array
    for (int i = 0; i < 26; i++) {
        std::cout << (char) (i + 97) << " (index=" << i << "):" << std::endl;
        for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++) {
                int flag = mono[i][x][y];
                if (flag) {
                    std::cout << "o";
                } else {
                    std::cout << " ";
                }
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}

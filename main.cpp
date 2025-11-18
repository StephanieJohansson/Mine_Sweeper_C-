#include <iostream>
#include "game.h"

int main() {
    std::cout << "=== MINESWEEPER ===" << std::endl;
    std::cout << "Creating a 8x8 board with 10 mines..." << std::endl;
    
    // create and start the game
    Game game(8, 8, 10);
    game.start();
    
    return 0;
}
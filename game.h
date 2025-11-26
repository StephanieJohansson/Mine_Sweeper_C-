#ifndef GAME_H
#define GAME_H

#include <memory>
#include "board.h"
#include <string>

// Manages the game flow
class Game {
public:
    Game(int rows, int cols, int mineCount);
    void start();

private:
    std::unique_ptr<Board> board;
    
    // Handles player input commands
    void handleCommand(const std::string& input);
    
    // VG functions for save/load
    void saveGame();
    bool showMainMenu();
    bool loadGameMenu();
};

#endif
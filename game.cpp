#include "game.h"
#include <iostream>
#include <sstream>

Game::Game(int rows, int cols, int mineCount) {
    board = std::make_unique<Board>(rows, cols, mineCount);
}

void Game::start() {
    std::cout << "\n=== MINESWEEPER===" << std::endl;
    std::cout << "Commands:" << std::endl;
    std::cout << "  r ROW COL  - Reveal cell (example: r a 1)" << std::endl;
    std::cout << "  f ROW COL  - Flag/unflag cell (example: f b 3)" << std::endl;
    std::cout << "  quit       - Quit the game" << std::endl;
    std::cout << "Rows: a-h, Columns: 1-8" << std::endl;
    std::cout << "\nGood luck!\n" << std::endl;
    
    while (true) {
        // Show the board
        board->print(std::cout);
        std::cout << std::endl;
        
        // Check if the game is over
        if (board->playerHitMine()) {
            std::cout << "BOOM! You hit a mine! Game over." << std::endl;
            break;
        }
        
        if (board->playerHasWon()) {
            std::cout << "Congratulations! You won the game!" << std::endl;
            break;
        }
        
        // Get input from the player
        std::cout << "Enter command: ";
        std::string input;
        std::getline(std::cin, input);
        
        if (input == "quit") {
            std::cout << "Thank you for playing!" << std::endl;
            break;
        }
        
        handleCommand(input);
    }
}

void Game::handleCommand(const std::string& input) {
    std::istringstream iss(input);
    std::string command;
    std::string rowStr;
    int col;
    
    iss >> command >> rowStr >> col;
    
    if (command.empty() || rowStr.empty()) {
        return; // Empty input
    }
    
    // Convert letter to row-number (a=0, b=1, c=2, ...)
    int row = -1;
    if (rowStr.length() == 1 && rowStr[0] >= 'a' && rowStr[0] <= 'z') {
        row = rowStr[0] - 'a';
    } else if (rowStr.length() == 1 && rowStr[0] >= 'A' && rowStr[0] <= 'Z') {
        row = rowStr[0] - 'A';
    }
    
    // Convert column from 1-indexed to 0-indexed
    col = col - 1;
    
    // Check if the coordinates are valid
    if (row == -1 || !board->isInsideBoard(row, col)) {
        std::cout << "Invalid coordinates! Use format like 'r a 1' (row a-h, column 1-8)." << std::endl;
        return;
    }
    
    if (command == "r" || command == "reveal") {
        board->revealCell(row, col);
    } else if (command == "f" || command == "flag") {
        board->toggleFlag(row, col);
    } else {
        std::cout << "Unknown command: " << command << std::endl;
        std::cout << "Use 'r ROW COL' to reveal or 'f ROW COL' to flag." << std::endl;
    }
}

std::pair<int, int> Game::parseCoordinates(const std::string& input) {
    // This function is not used in the current implementation
    return {0, 0};
}
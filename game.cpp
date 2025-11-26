#include "game.h"
#include <iostream>
#include <sstream>
#include <limits> // For std::numeric_limits

Game::Game(int rows, int cols, int mineCount) {
    board = std::make_unique<Board>(rows, cols, mineCount);
}

void Game::start() {
    // Visa huvudmeny först
    if (!showMainMenu()) {
        return; // Spelaren valde att avsluta
    }
    
    std::cout << "\n=== MINESWEEPER===" << std::endl;
    std::cout << "Commands:" << std::endl;
    std::cout << "  r ROW COL  - Reveal cell (example: r a 1)" << std::endl;
    std::cout << "  f ROW COL  - Flag/unflag cell (example: f b 3)" << std::endl;
    std::cout << "  save       - Save current game" << std::endl;
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

// Handles player input commands
void Game::handleCommand(const std::string& input) {
    std::istringstream iss(input);
    std::string command;
    
    iss >> command;
    
    if (command.empty()) {
        return; // Empty input
    }
    
    // Handle save command first (no coordinates needed)
    if (command == "save") {
        saveGame();
        return;
    }
    
    // For other commands, read coordinates
    std::string rowStr;
    int col;
    iss >> rowStr >> col;
    
    if (rowStr.empty()) {
        std::cout << "Missing coordinates! Use format like 'r a 1' or 'f b 3'." << std::endl;
        return;
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
    
    // Execute the command
    if (command == "r" || command == "reveal") {
        board->revealCell(row, col);
    } else if (command == "f" || command == "flag") {
        board->toggleFlag(row, col);
    } else {
        std::cout << "Unknown command: " << command << std::endl;
        std::cout << "Use 'r X Y' to reveal, 'f X Y' to flag, or 'save' to save game." << std::endl;
    }
}

// Save the current game state
void Game::saveGame() {
    std::string filename;
    std::cout << "Enter filename to save (ex: mygame.txt): ";
    std::getline(std::cin, filename); // Ta bort cin.ignore() - problemet är med olika input-metoder
    
    if (filename.empty()) {
        std::cout << "Save cancelled." << std::endl;
        return;
    }
    
    try {
        board->saveToFile(filename);
        std::cout << "Game saved successfully to '" << filename << "'!" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Error saving game: " << e.what() << std::endl;
    }
}

// Show main menu
bool Game::showMainMenu() {
    std::cout << "\n=== MINESWEEPER - MAIN MENU ===" << std::endl;
    std::cout << "1. New Game" << std::endl;
    std::cout << "2. Load Game" << std::endl;
    std::cout << "3. Quit" << std::endl;
    std::cout << "Enter choice (1-3): ";
    
    std::string choice;
    std::getline(std::cin, choice);
    
    if (choice == "1") {
        // Keep current board for new game
        return true;
    } else if (choice == "2") {
        return loadGameMenu();
    } else if (choice == "3") {
        std::cout << "Goodbye!" << std::endl;
        return false;
    } else {
        std::cout << "Invalid choice. Starting new game..." << std::endl;
        return true;
    }
}

// Load game menu
bool Game::loadGameMenu() {
    std::string filename;
    std::cout << "Enter filename to load (ex: mygame.txt): ";
    std::getline(std::cin, filename); 
    
    std::cout << "DEBUG: Trying to load file: '" << filename << "'" << std::endl;
    
    // If the user enters an empty filename, cancel loading
    if (filename.empty()) {
        std::cout << "Load cancelled. Starting new game..." << std::endl;
        return true;
    }
    
    // Attempt to load the game
    try {
        std::cout << "DEBUG: Attempting to load from file..." << std::endl;
        auto loadedBoard = Board::loadFromFile(filename);
        std::cout << "DEBUG: File loaded successfully, moving to board..." << std::endl;
        board = std::move(loadedBoard);
        std::cout << "Game loaded successfully from '" << filename << "'!" << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cout << "Error loading game: " << e.what() << std::endl;
        std::cout << "Starting new game instead..." << std::endl;
        return true;
    }
}
#include "board.h"
#include <iostream>
#include <random>
#include <fstream>
#include <stdexcept>

// Constructor to initialize the board with given dimensions and mine count

Board::Board(int rows, int cols, int mineCount) 
    : rows(rows), cols(cols), mineCount(mineCount) {
    
    initEmptyGrid();
    placeMinesRandomly();
    calculateAdjacentMines();
}

void Board::initEmptyGrid() {
    // create an empty grid with all cells
    grid.resize(rows);
    for (int r = 0; r < rows; r++) {
        grid[r].resize(cols);
        for (int c = 0; c < cols; c++) {
            grid[r][c] = Cell(); // Empty cell
        }
    }
}

// Randomly place mines on the board
void Board::placeMinesRandomly() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> rowDist(0, rows - 1);
    std::uniform_int_distribution<> colDist(0, cols - 1);
    
    int placedMines = 0;
    while (placedMines < mineCount) {
        int r = rowDist(gen);
        int c = colDist(gen);
        
        // If the cell does not already have a mine, place one
        if (!grid[r][c].HasMine) {
            grid[r][c].HasMine = true;
            placedMines++;
        }
    }
}

// Calculate number of adjacent mines for each cell
void Board::calculateAdjacentMines() {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (!grid[r][c].HasMine) {
                grid[r][c].AdjacentMines = countAdjacentMines(r, c);
            }
        }
    }
}

// Count adjacent mines for a given cell
int Board::countAdjacentMines(int row, int col) const {
    int count = 0;
    
    // Check all 8 adjacent cells
    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) continue; // Skip the cell itself
            
            int newRow = row + dr;
            int newCol = col + dc;
            
            if (inBounds(newRow, newCol) && grid[newRow][newCol].HasMine) {
                count++;
            }
        }
    }
    
    return count;
}

// Check if coordinates are inside the board
bool Board::inBounds(int r, int c) const {
    return r >= 0 && r < rows && c >= 0 && c < cols;
}

// Reveal a cell
void Board::revealCell(int row, int col) {
    if (!inBounds(row, col) || grid[row][col].IsRevealed || grid[row][col].IsFlagged) {
        return;
    }
    
    grid[row][col].IsRevealed = true;
    
    // If the player hits a mine
    if (grid[row][col].HasMine) {
        hitMine = true;
        return;
    }
    
    // If the cell has 0 adjacent mines, reveal all adjacent cells (flood fill)
    if (grid[row][col].AdjacentMines == 0) {
        floodReveal(row, col);
    }
}

void Board::floodReveal(int r, int c) {
    // Reveal all adjacent cells recursively
    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) continue;
            
            int newRow = r + dr;
            int newCol = c + dc;
            
            if (inBounds(newRow, newCol) && !grid[newRow][newCol].IsRevealed && !grid[newRow][newCol].IsFlagged) {
                grid[newRow][newCol].IsRevealed = true;
                
                // If this cell also has 0 adjacent mines, continue flood fill
                if (grid[newRow][newCol].AdjacentMines == 0 && !grid[newRow][newCol].HasMine) {
                    floodReveal(newRow, newCol);
                }
            }
        }
    }
}

// Toggle flag on a cell
void Board::toggleFlag(int row, int col) {
    if (!inBounds(row, col) || grid[row][col].IsRevealed) {
        return;
    }
    
    grid[row][col].IsFlagged = !grid[row][col].IsFlagged;
}

// Check if a cell has a mine
bool Board::hasMine(int row, int col) const {
    return inBounds(row, col) && grid[row][col].HasMine;
}

// Check if a cell is revealed
bool Board::isRevealed(int row, int col) const {
    return inBounds(row, col) && grid[row][col].IsRevealed;
}


// Check if a cell is flagged
bool Board::isFlagged(int row, int col) const {
    return inBounds(row, col) && grid[row][col].IsFlagged;
}

// Check if coordinates are inside the board
bool Board::isInsideBoard(int row, int col) const {
    return inBounds(row, col);
}

// Get number of adjacent mines for a cell
int Board::getAdjacentMines(int row, int col) const {
    if (!inBounds(row, col)) return 0;
    return grid[row][col].AdjacentMines;
}

// Check if the player has hit a mine
bool Board::playerHitMine() const {
    return hitMine;
}

// Check if the player has won
bool Board::playerHasWon() const {
    // The player wins if all cells without mines are revealed
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (!grid[r][c].HasMine && !grid[r][c].IsRevealed) {
                return false; // There are still hidden cells without mines
            }
        }
    }
    return true;
}

void Board::print(std::ostream& os) const {
    // Print column indices
    os << "   ";
    for (int c = 0; c < cols; c++) {
        os << (c + 1) << " ";  // 1-indexerad som i exemplet
    }
    os << std::endl;
    
    // Print each row with letter labels
    for (int r = 0; r < rows; r++) {
        char rowLabel = 'a' + r;  // a, b, c, d...
        os << rowLabel << ": ";
        for (int c = 0; c < cols; c++) {
            const Cell& cell = grid[r][c];
            
            if (cell.IsFlagged) {
                os << "F ";
            } else if (!cell.IsRevealed) {
                os << "? ";
            } else if (cell.HasMine) {
                os << "* ";
            } else if (cell.AdjacentMines > 0) {
                os << cell.AdjacentMines << " ";
            } else {
                os << "  ";
            }
        }
        os << std::endl;
    }
}

// Save the board state to a file
void Board::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for writing: " + filename);
    }
    
    // Save the board dimensions and mine count
    file << rows << " " << cols << " " << mineCount << std::endl;
    
    // Save whether the player has hit a mine
    file << (hitMine ? 1 : 0) << std::endl;
    
    // Save each cell on the board
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            const Cell& cell = grid[r][c];
            file << (cell.HasMine ? 1 : 0) << " "
                 << (cell.IsRevealed ? 1 : 0) << " "
                 << (cell.IsFlagged ? 1 : 0) << " "
                 << cell.AdjacentMines << std::endl;
        }
    }
    
    file.close();
}

std::unique_ptr<Board> Board::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for reading: " + filename);
    }
    
// this was a problematic part, because it was a weak check that could lead to
// undetected errors later on:
// file >> rows >> cols >> mineCount;
// if (file.fail()) //

    // Read board dimensions and mine count
    int rows, cols, mineCount;
    if (!(file >> rows >> cols >> mineCount)) {
        throw std::runtime_error("Could not read board dimensions from file");
    }
    
    if (rows <= 0 || cols <= 0 || mineCount < 0 || rows > 50 || cols > 50) {
        throw std::runtime_error("Invalid board dimensions in save file");
    }
    
    // Read hitMine status
    int hitMineFlag;
    if (!(file >> hitMineFlag)) {
        throw std::runtime_error("Could not read game state from file");
    }
    
    // Create a new Board object and manually set its values
    auto board = std::make_unique<Board>(rows, cols, 0); // Create with 0 mines initially
    
    // Override the generated state with loaded state
    board->mineCount = mineCount;
    board->hitMine = (hitMineFlag == 1);
    
    // Clear and resize grid
    board->grid.clear();
    board->grid.resize(rows);
    for (int r = 0; r < rows; r++) {
        board->grid[r].resize(cols);
    }
    
    // Read each cell's data with better error checking
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            int hasMine, isRevealed, isFlagged, adjacentMines;
            
            if (!(file >> hasMine >> isRevealed >> isFlagged >> adjacentMines)) {
                throw std::runtime_error("Corrupted save file data at cell (" + 
                                       std::to_string(r) + "," + std::to_string(c) + ")");
            }
            
            // Validate data ranges
            if (hasMine < 0 || hasMine > 1 || isRevealed < 0 || isRevealed > 1 || 
                isFlagged < 0 || isFlagged > 1 || adjacentMines < 0 || adjacentMines > 8) {
                throw std::runtime_error("Invalid cell data in save file");
            }
            
            Cell& cell = board->grid[r][c];
            cell.HasMine = (hasMine == 1);
            cell.IsRevealed = (isRevealed == 1);
            cell.IsFlagged = (isFlagged == 1);
            cell.AdjacentMines = adjacentMines;
        }
    }
    
    file.close();
    return board;
}
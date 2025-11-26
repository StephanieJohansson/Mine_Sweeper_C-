#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <memory>
#include "cell.h"

// Game logics

class Board {
public:
    Board(int rows, int cols, int mineCount);

    void revealCell(int row, int col);
    void toggleFlag(int row, int col);

    bool hasMine(int row, int col) const;
    bool isRevealed(int row, int col) const;
    bool isFlagged(int row, int col) const;

    bool isInsideBoard(int row, int col) const;
    int getAdjacentMines(int row, int col) const;

    bool playerHitMine() const;
    bool playerHasWon() const;

    // Print the board to an output stream
    void print(std::ostream& os) const;

        // To save to file and load from file
        void saveToFile(const std::string& filename) const;
        static std::unique_ptr<Board> loadFromFile(const std::string& filename);

    int getRows() const { 
        return rows; }
    int getCols() const { 
        return cols; }

        
private: // Board dimensions and mine count
    int rows;
    int cols;
        int mineCount;
        bool hitMine = false;

        // vector of vector of Cells representing the grid
        std::vector<std::vector<Cell>> grid;

        // Helper functions
        void initEmptyGrid();
        void placeMinesRandomly();
        void calculateAdjacentMines();
        int countAdjacentMines(int row, int col) const;
        bool inBounds(int r, int c) const;

        void floodReveal(int r, int c); // Reveals all connected empty cells

};

#endif
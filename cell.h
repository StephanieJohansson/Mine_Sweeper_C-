#ifndef CELL_H
#define CELL_H

// Represents a single cell on the Minesweeper board
class Cell {
public:
    bool HasMine = false;
    bool IsRevealed = false;
    bool IsFlagged = false;
    int AdjacentMines = 0;
};

#endif 
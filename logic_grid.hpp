#pragma once
#include <cstdint>
/*
#include <cstdlib>
#include <string>
#include <iostream>
*/

// first bit: isAlive, last 4 bits: no. of neighbors
typedef uint8_t Cell;

class LGrid
{
private:
    Cell *cellArray;
    int widthCells;
    int heightCells;
    // set n to 1
    void set(Cell *num, int n)
    {
        *num = *num | ((Cell)1 << n);
    }
    // set n to 0
    void clear(Cell *num, int n)
    {
        *num = *num & ~((Cell)1 << n);
    }
    // switch n from 1 to 0 or from 0 to 1
    void toggle(Cell *num, int n)
    {
        *num = *num ^ ((int)1 << n);
    }
    // get n
    bool read(Cell *num, int n)
    {
        return (bool)(*num & ((Cell)1 << n));
    }

public:
    LGrid();
    LGrid(int widthCells, int heightCells);
    ~LGrid();
    // void printGrid();
    int getIndex(int x, int y);
    void calculateBorders();
    void calculateNeighbors();
    void updateCells();
    void nextFrame()
    {
        calculateBorders();
        calculateNeighbors();
        updateCells();
        // printGrid();
    }
    bool isAlive(int x, int y) { return read(&cellArray[getIndex(x, y)], 7); }
    bool isAlive(Cell *num) { return read(num, 7); }
    void setCell(int, int, bool);
    void clearGrid();
};
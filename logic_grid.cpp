#include "logic_grid.hpp"


LGrid::LGrid()
    : cellArray(nullptr), widthCells(0), heightCells(0) {}

LGrid::LGrid(int widthCells, int heightCells)
    : cellArray(new Cell[widthCells * heightCells]), widthCells(widthCells), heightCells(heightCells)
{
    for (int i = 0; i < widthCells * heightCells; i++)
    {
        cellArray[i] = 0;
    }
}

LGrid::~LGrid() { delete[] cellArray; }
int LGrid::getIndex(int x, int y) { return y * widthCells + x; }

void LGrid::calculateBorders()
{
    Cell *oldNum = nullptr;
    // corners:

    // #.
    // ..
    oldNum = &cellArray[getIndex(0, 0)];
    *oldNum = (isAlive(oldNum) * 0x80) + isAlive(1, 0) + isAlive(1, 1) + isAlive(0, 1);

    // .#
    // ..
    oldNum = &cellArray[getIndex(widthCells - 1, 0)];
    *oldNum = (isAlive(oldNum) * 0x80) + isAlive(widthCells - 2, 0) + isAlive(widthCells - 2, 1) + isAlive(widthCells - 1, 1);

    // ..
    // .#
    oldNum = &cellArray[getIndex(widthCells - 1, heightCells - 1)];
    *oldNum = (isAlive(oldNum) * 0x80) + isAlive(widthCells - 1, heightCells - 2) + isAlive(widthCells - 2, heightCells - 2) + isAlive(widthCells - 2, heightCells - 1);

    // ..
    // #.
    oldNum = &cellArray[getIndex(0, heightCells - 1)];
    *oldNum = (isAlive(oldNum) * 0x80) + isAlive(0, heightCells - 2) + isAlive(1, heightCells - 2) + isAlive(1, heightCells - 1);

    // sides:
    // horizontal
    for (int j = 1; j < widthCells - 1; j++)
    {
        // up
        oldNum = &cellArray[getIndex(j, 0)];
        *oldNum = (isAlive(oldNum) * 0x80) + isAlive(j - 1, 0) + isAlive(j + 1, 0) + isAlive(j - 1, 1) + isAlive(j, 1) + isAlive(j + 1, 1);

        // down
        oldNum = &cellArray[getIndex(j, heightCells - 1)];
        *oldNum = (isAlive(oldNum) * 0x80) + isAlive(j - 1, heightCells - 1) + isAlive(j + 1, heightCells - 1) + isAlive(j - 1, heightCells - 2) + isAlive(j, heightCells - 2) + isAlive(j + 1, heightCells - 2);
    }

    // vertical
    for (int i = 1; i < heightCells - 1; i++)
    {
        // left
        oldNum = &cellArray[getIndex(0, i)];
        *oldNum = (isAlive(oldNum) * 0x80) + isAlive(0, i - 1) + isAlive(0, i + 1) + isAlive(1, i - 1) + isAlive(1, i) + isAlive(1, i + 1);

        // right
        oldNum = &cellArray[getIndex(widthCells - 1, i)];
        *oldNum = (isAlive(oldNum) * 0x80) + isAlive(widthCells - 1, i - 1) + isAlive(widthCells - 1, i + 1) + isAlive(widthCells - 2, i - 1) + isAlive(widthCells - 2, i) + isAlive(widthCells - 2, i + 1);
    }
}

void LGrid::calculateNeighbors()
{

    Cell *oldNum = nullptr;

    for (int i = 1; i < heightCells - 1; i++)
    {
        for (int j = 1; j < widthCells - 1; j++)
        {
            oldNum = &cellArray[getIndex(j, i)];
            *oldNum = (isAlive(oldNum) * 0x80) + isAlive(j - 1, i - 1) + isAlive(j, i - 1) + isAlive(j + 1, i - 1) + isAlive(j - 1, i) + isAlive(j + 1, i) + isAlive(j - 1, i + 1) + isAlive(j, i + 1) + isAlive(j + 1, i + 1);
        }
    }
}

void LGrid::updateCells()
{
    Cell currNeigbors = 0;
    Cell *currCell = nullptr;
    for (int i = 0; i < heightCells; i++)
    {
        for (int j = 0; j < widthCells; j++)
        {
            currCell = &cellArray[getIndex(j, i)];
            currNeigbors = *currCell;
            clear(&currNeigbors, 7);

            if (isAlive(currCell) && (currNeigbors < 2 || currNeigbors > 3))
            {
                clear(currCell, 7);
            }
            if (!isAlive(currCell) && (currNeigbors == 3))
            {
                set(currCell, 7);
            }
        }
    }
}

void LGrid::setCell(int x, int y, bool alive = true)
{
    if (alive)
    {
        set(&cellArray[getIndex(x, y)], 7);
    }
    else
    {
        clear(&cellArray[getIndex(x, y)], 7);
    }
}
void LGrid::clearGrid(){
    for (int i = 0; i < heightCells*widthCells; i++)
    {
        clear(&cellArray[i], 7);
    }
    
    
}

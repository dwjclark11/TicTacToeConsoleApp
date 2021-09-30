#pragma once
#include "Cell.h"

class Board
{
private:
    int boardSize;

public:
    Cell gameGrid[3][3];

    Board(int board_size)
    {
        //
        boardSize = board_size;

        // Set the numbers to the cells
        for (int i = 0; i < boardSize; i++)
        {
            for (int j = 0; j < boardSize; j++)
            {
                gameGrid[i][j].SetRowNumber(i);
                gameGrid[i][j].SetColNumber(j);
            }
        }
    }
    ~Board() {}

    int GetBoardSize() { return boardSize; }
};

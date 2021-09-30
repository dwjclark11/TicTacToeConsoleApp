#pragma once

class Cell
{
private:
    char cellLetter;
    bool isOccupied;
    int playerNum;
    int rowNumber;
    int colNumber;

public:
    Cell()
    {
        cellLetter = '\0';
        isOccupied = false;
        playerNum = 0;
        rowNumber = 0;
        colNumber = 0;
    }
    Cell(int row, int col)
    {
        cellLetter = '\0';
        isOccupied = false;
        playerNum = 0;
        rowNumber = row;
        colNumber = col;
    }

    bool& IsCellOccupied() { return isOccupied; }
    char GetCellLetter() { return cellLetter; }
    void SetCellLetter(char letter) { cellLetter = letter; }
    int GetRowNumber() { return rowNumber; }
    int GetColNumber() { return colNumber; }
    void SetRowNumber(int row) { rowNumber = row; }
    void SetColNumber(int col) { colNumber = col; }
    int GetPlayerNumber() { return playerNum; }
    void SetPlayerNumber(int num) { playerNum = num; }
};


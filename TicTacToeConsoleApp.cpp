#include <iostream>
#include "Board.h"
#include <Windows.h>
#include <stdio.h>

// Constants
const int screenWidth = 120;
const int screenHeight = 30;

// Global board 
Board gameHolder(3);

// Create Screen Buffer
wchar_t* screen = new wchar_t[screenWidth * screenHeight];

void PrintBoard(Board board)
{
    swprintf(&screen[7 * screenWidth + 50], L" =============");
    swprintf(&screen[8 * screenWidth + 50], L" |Game Board |");
    swprintf(&screen[9 * screenWidth + 50], L" =============");

    // Print col numbers
    swprintf(&screen[10 * screenWidth + 50], L"   0   1   2");

    for (int i = 0; i < board.GetBoardSize(); i++)
    {
        swprintf(&screen[(11 + i*2) * screenWidth + 50], L" +---+---+---+");
        swprintf(&screen[(12 + i*2) * screenWidth + 49], L"%d", i);

        for (int j = 0; j < board.GetBoardSize(); j++)
        {
            if (board.gameGrid[i][j].IsCellOccupied())
            {   
                if (board.gameGrid[i][j].GetPlayerNumber() == 1)
                {
                    swprintf(&screen[(12 + i * 2) * screenWidth + 53 + (j * 4)], L"X");
                }
                else if (board.gameGrid[i][j].GetPlayerNumber() == 2)
                {
                    swprintf(&screen[(12 + i * 2) * screenWidth + 53 + (j * 4)], L"O");
                }
            }
            else
            {
                swprintf(&screen[(12 + i * 2) * screenWidth + 53 + (j * 4)], L"");
            }
            swprintf(&screen[(12 + i * 2) * screenWidth + 51 + (j * 4)], L"|");

            // Close the box
            if (j == board.GetBoardSize() - 1)
                swprintf(&screen[(12 + i * 2) * screenWidth + 51 + (board.GetBoardSize() * 4)], L"|");

        }
    }
    swprintf(&screen[17 * screenWidth + 50], L" +---+---+---+");
}

bool PlaceGameLetter(int player, int row, int col, Board& board)
{
    // If the cell is not currently occupied
    if (!board.gameGrid[row][col].IsCellOccupied())
    {
        board.gameGrid[row][col].SetPlayerNumber(player);
        board.gameGrid[row][col].IsCellOccupied() = true;
        return true;
    }
    // Cell is occupied
    return false;
}

bool CheckForWin(Board& board, const int& player)
{
    if (// Check Horizontal
        (board.gameGrid[0][0].GetPlayerNumber() == player && board.gameGrid[0][1].GetPlayerNumber() == player && board.gameGrid[0][2].GetPlayerNumber() == player) ||
        (board.gameGrid[1][0].GetPlayerNumber() == player && board.gameGrid[1][1].GetPlayerNumber() == player && board.gameGrid[1][2].GetPlayerNumber() == player) ||
        (board.gameGrid[2][0].GetPlayerNumber() == player && board.gameGrid[2][1].GetPlayerNumber() == player && board.gameGrid[2][2].GetPlayerNumber() == player) ||
        // Check Vertical
        (board.gameGrid[0][0].GetPlayerNumber() == player && board.gameGrid[1][0].GetPlayerNumber() == player && board.gameGrid[2][0].GetPlayerNumber() == player) ||
        (board.gameGrid[0][1].GetPlayerNumber() == player && board.gameGrid[1][1].GetPlayerNumber() == player && board.gameGrid[2][1].GetPlayerNumber() == player) ||
        (board.gameGrid[0][2].GetPlayerNumber() == player && board.gameGrid[1][2].GetPlayerNumber() == player && board.gameGrid[2][2].GetPlayerNumber() == player) ||
        // Check for diagonals
        (board.gameGrid[0][0].GetPlayerNumber() == player && board.gameGrid[1][1].GetPlayerNumber() == player && board.gameGrid[2][2].GetPlayerNumber() == player) ||
        (board.gameGrid[2][0].GetPlayerNumber() == player && board.gameGrid[1][1].GetPlayerNumber() == player && board.gameGrid[0][2].GetPlayerNumber() == player))
        return true;

    return false;
}

bool CheckForFullBoard(Board& board)
{
    for (int i = 0; i < board.GetBoardSize(); i++)
    {
        for (int j = 0; j < board.GetBoardSize(); j++)
        {
            if (!board.gameGrid[i][j].IsCellOccupied())
            {
                return false;
            }
        }
    }

    return true;
}

int main()
{
    // Set the Screen buffer Values
    for (int i = 0; i < screenWidth * screenHeight; i++) screen[i] = L' ';
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD bytesWritten = 0;

    // Create variables and set default values
    int player = 1;
    int row = -1;
    int col = -1;

    bool gamePlaying = true;
    bool rowSet = false, colSet = false;
    bool keyRight = false, keyLeft = false, keyRightPrev = false, keyLeftPrev = false, enterKey = false, enterKeyPrev = false;
    bool noWinner = true, checkWin = false, cellOccupied = false, fullBoard = false;

    // Run the GameLoop
    while (gamePlaying)
    {
        row = 0;
        col = 0;
        // While there is no winner
        while (noWinner)
        {
            // Set input/key values
            keyRight = (GetAsyncKeyState(VK_RIGHT) & 0x8000);
            keyLeft = (GetAsyncKeyState(VK_LEFT) & 0x8000);
            enterKey = (GetAsyncKeyState(VK_RETURN) & 0x8000);

            // Clear Screen
            for (int i = 0; i < screenWidth * screenHeight; i++) screen[i] = L' ';

            // Print TitleBar
            swprintf(&screen[1 * screenWidth + 15], L"=========================================================================================");
            swprintf(&screen[2 * screenWidth + 15], L"|                   Welcome to this Simple Console Tic Tac Toe Game!                    |");
            swprintf(&screen[3 * screenWidth + 15], L"=========================================================================================");
            
            PrintBoard(gameHolder);
            swprintf(&screen[4 * screenWidth + 50], L"Player# %d's Turn!", player);
            swprintf(&screen[6 * screenWidth + 43], L"Please enter the row number: %d", row);
            
            // If left key pressed
            if (keyLeft && !keyLeftPrev)
            {
                if (!rowSet)
                    row--;
                else
                    col--;
            }
            // If right key pressed
            else if (keyRight && !keyRightPrev)
            {
                if (!rowSet)
                    row++;
                else
                    col++;
            }
            // only allow one increment per key press
            keyRightPrev = keyRight;
            keyLeftPrev = keyLeft;

            // clamp row to 0-2
            if (row <= 0) row = 0;
            else if (row >= 2) row = 2;

            // clamp col to 0-2
            if (col <= 0) col = 0;
            else if (col >= 2) col = 2;

            // Enter key pressed
            if (enterKey && !enterKeyPrev)
            {
                if (!rowSet)
                    rowSet = true;
                else
                    colSet = true;
            }
            enterKeyPrev = enterKey;

            if (rowSet)
                swprintf(&screen[6 * screenWidth + 43], L"Please enter the col number: %d", col);

            // if row and col are chosen, try to place the player character
            if (rowSet && colSet)
            {
                // Check to see if the cell is already occupied
                if (PlaceGameLetter(player, row, col, gameHolder))
                {
                    checkWin = true;
                    cellOccupied = false;
                }
                else
                {
                    cellOccupied = true;
                }
                // Reset selection
                rowSet = false;
                colSet = false;
                row = 0;
                col = 0;
            }
            // If player tries to select an occupied cell
            if (cellOccupied) swprintf(&screen[20 * screenWidth + 28], L" That cell is currently occupied, Please select another cell ");

            // Check to see if the current player has won
            if (CheckForWin(gameHolder, player))
            {
                noWinner = false;
            }
            else if (checkWin)
            {
                if (player == 1) player = 2;
                else if (player == 2) player = 1;
                checkWin = false;
            }

            // Check for full Game Board and no winner
            if (CheckForFullBoard(gameHolder))
            {
                noWinner = false;
                fullBoard = true;
            }

            // If there is a winner, print which player has won and break the loop
            if (!noWinner)
            {
                if(!fullBoard) swprintf(&screen[20 * screenWidth + 45], L"    PLAYER# %d WINS!!    ", player);
                else swprintf(&screen[20 * screenWidth + 45], L"CAT'S GAME --> NO WINNER");

                swprintf(&screen[21 * screenWidth + 42], L" PRESS 'SPACE' TO PLAY AGAIN ");
                swprintf(&screen[22 * screenWidth + 42], L"   PRESS 'ESC' TO QUIT GAME  ");
                PrintBoard(gameHolder);
            }

            swprintf(&screen[25 * screenWidth + 27], L"PRESS LEFT/RIGHT KEYS TO INCREMENT/DECREMENT SELECTED ROWS/COLS");

            // Draw Closing Bar
            swprintf(&screen[27 * screenWidth + 15], L"=========================================================================================");

            // Draw Left Bar
            for (int i = 0; i < screenHeight - 7; i++) 
                swprintf(&screen[(4 + i) * screenWidth + 15], L"|");

            // Draw Right Bar
            for (int i = 0; i < screenHeight - 7; i++)
                swprintf(&screen[(4 + i) * screenWidth + 103], L"|");

            // Render Frame
            WriteConsoleOutputCharacter(hConsole, screen, screenWidth * screenHeight, { 0,0 }, &bytesWritten);
        }
        // Wait here for space bar to continue or esc to quit game
        while ((GetAsyncKeyState(VK_SPACE) & 0x8000) == 0)
        {
            // If escape is pressed leave game
            if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
            {
                gamePlaying = false;
                break;
            }
        }
        // Reset winner status
        noWinner = true;

        // Clear the board
        for (int i = 0; i < gameHolder.GetBoardSize(); i++)
        {
            for (int j = 0; j < gameHolder.GetBoardSize(); j++)
            {
                gameHolder.gameGrid[i][j].IsCellOccupied() = false;
                gameHolder.gameGrid[i][j].SetPlayerNumber(0);
            }
        }
    }

    return 0;
}
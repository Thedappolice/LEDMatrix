#include "RIPTetris_No_Chip.h"

// function declaration

Tetris_No_Chip::Tetris_No_Chip()
{
}

Tetris_No_Chip::~Tetris_No_Chip()
{
}

void Tetris_No_Chip::Char( char input, unsigned long duration)
{
    // Predefined symbols
    int symbols[4][8][8] = {
        {// Symbol '1'
         {0, 0, 0, 1, 1, 0, 0, 0},
         {0, 0, 1, 1, 1, 0, 0, 0},
         {0, 1, 1, 1, 1, 0, 0, 0},
         {0, 0, 0, 1, 1, 0, 0, 0},
         {0, 0, 0, 1, 1, 0, 0, 0},
         {0, 0, 0, 1, 1, 0, 0, 0},
         {0, 1, 1, 1, 1, 1, 1, 0},
         {0, 1, 1, 1, 1, 1, 1, 0}},
        {// Symbol '2'
         {0, 0, 1, 1, 1, 1, 0, 0},
         {0, 1, 1, 1, 1, 1, 1, 0},
         {0, 1, 1, 0, 0, 1, 1, 0},
         {0, 0, 0, 0, 1, 1, 1, 0},
         {0, 0, 0, 1, 1, 1, 0, 0},
         {0, 0, 1, 1, 1, 0, 0, 0},
         {0, 1, 1, 1, 1, 1, 1, 0},
         {0, 1, 1, 1, 1, 1, 1, 0}},
        {// Symbol '3'
         {0, 0, 1, 1, 1, 1, 0, 0},
         {0, 1, 1, 1, 1, 1, 1, 0},
         {0, 0, 0, 0, 0, 1, 1, 0},
         {0, 0, 1, 1, 1, 1, 0, 0},
         {0, 0, 1, 1, 1, 1, 0, 0},
         {0, 0, 0, 0, 0, 1, 1, 0},
         {0, 1, 1, 1, 1, 1, 1, 0},
         {0, 0, 1, 1, 1, 1, 0, 0}},
        {// Symbol 'E' (End)
         {0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0},
         {1, 1, 1, 0, 0, 0, 0, 1},
         {1, 0, 0, 0, 0, 0, 0, 1},
         {1, 1, 0, 0, 1, 0, 0, 1},
         {1, 0, 0, 1, 0, 1, 1, 0},
         {1, 1, 1, 1, 0, 1, 0, 1},
         {0, 0, 0, 0, 0, 0, 0, 0}}};

    // Map the input character to the symbol index
    int symbolIndex = -1;
    switch (input)
    {
    case '1':
        symbolIndex = 0;
        break;
    case '2':
        symbolIndex = 1;
        break;
    case '3':
        symbolIndex = 2;
        break;
    case 'E':
        symbolIndex = 3;
        break;
    default:
        break;
    }

    // Display the symbol on the LED matrix
    if (symbolIndex != -1)
    {
        (duration > 0) ? LM.Symbol(symbols[symbolIndex], duration) : LM.Symbol(symbols[symbolIndex]);
    }

    // Display the symbol on the LED matrix
    if (symbolIndex != -1)
    {
        (duration > 0) ? Char(symbols[symbolIndex], duration) : Char(symbols[symbolIndex]);
    }
}

void Tetris_No_Chip::alterShape(int mode)
{
}

void Tetris_No_Chip::clearRow(int row)
{
}

void Tetris_No_Chip::shiftRowsDown(int startRow)
{
}

void Tetris_No_Chip::scanAndClearGrid()
{
}

void Tetris_No_Chip::checkInput()
{
}

void Tetris_No_Chip::showEndAnimation()
{
}

void Tetris_No_Chip::sendScore(int score)
{
}

void Tetris_No_Chip::setup()
{
}

void Tetris_No_Chip::loop()
{
}

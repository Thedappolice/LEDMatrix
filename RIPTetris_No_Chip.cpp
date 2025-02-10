#include "RIPTetris_No_Chip.h"

// function declaration

Tetris_No_Chip::Tetris_No_Chip()
{
    topLM = new LEDMatrix<8, 8>();
    botLM = new LEDMatrix<8, 8>();
}

Tetris_No_Chip::~Tetris_No_Chip()
{
}

void Tetris_No_Chip::Symbol( char input, unsigned long duration)
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

    memcpy(displayMatrix, symbols[symbolIndex], sizeof(displayMatrix));

    gatherAndDisplay(0);
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

void Tetris_No_Chip::gatherAndDisplay(bool symbol)
{
    if(symbol)
    {
        LMbot.Symbol(displayMatrix);
    }
    if (!end)
    { // Copy the stable grid into the display grid
        memcpy(grid.display, grid.stable, sizeof(grid.stable));

        // Add the active shape to the display grid
        for (int i = 0; i < 4; i++)
        {
            grid.display[currentShape.coordinates[i][0]][currentShape.coordinates[i][1]] = 1;
        }
    }

    // Update the top and bottom matrix displays
    for (int i = 0; i < GRID_WIDTH; i++)
    {
        memcpy(grid.top[i], &grid.display[i + 2][0], sizeof(grid.top[i]));                    // Top matrix
        memcpy(grid.bottom[i], &grid.display[i + GRID_WIDTH + 2][0], sizeof(grid.bottom[i])); // Bottom matrix
    }

    // Send updated data to the LED matrices
    LMtop.Symbol(grid.top, 2);
    LMbot.Symbol(grid.bottom, 2);
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

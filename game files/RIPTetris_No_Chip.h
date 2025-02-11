// file.h
#ifndef RIPTstris_NO_CHIP_H // Include guard
#define RIPTetris_NO_CHIP_H

#include <Arduino.h>

// Function prototype

class Tetris_No_Chip
{
public:
    Tetris_No_Chip();

    ~Tetris_No_Chip();

    void Char(LEDMatrix<8, 8> &LM, char input, unsigned long duration = 0);

    void alterShape(int mode);

    void genShape();

    void clearRow(int row);

    void shiftRowsDown(int startRow);

    void scanAndClearGrid();

    void checkInput();

    void gatherAndDisplay();

    void showEndAnimation();

    void sendScore(int score);

    void gameLoop();

private:
    int displayMatrix[][8];
    LEDMatrix<8, 8> *topLM;
    LEDMatrix<8, 8> *botLM;

    int shape[4][2];       // Relative positions of blocks in the shape
    int coordinates[4][2]; // Absolute positions of blocks on the grid
    bool active;           // Whether the shape is currently active

    // Timing Variables
    unsigned long lastInputTime = 0;   // Last time input was processed
    unsigned long inputInterval = 100; // Interval between inputs (ms)
    unsigned long prev = 0;            // Previous time for game interval
    unsigned long interval = 500;      // Time between automatic block drops (ms)

    // Track the previous state of the rotate button
    bool previousRotateState = LOW;

    // Game State Variables
    int score = 0;      // Current score
    bool end = false;   // Whether the game is over
    bool ended = false; // Whether the end animation has been shown
}

#endif

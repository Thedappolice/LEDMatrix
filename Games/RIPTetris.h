#ifndef RIPTETRIS_H
#define RIPTETRIS_H

#include <Arduino.h>
#include <LEDMatrix.h>

// Grid Constants
#define GRID_WIDTH 8
#define GRID_HEIGHT (GRID_WIDTH * 2)
#define CHECKING_HEIGHT (GRID_HEIGHT + 2)

enum direction
{
    DOWN,
    LEFT,
    RIGHT,
    ROTATE
};

// Class Definition
class RIPTetris
{
public:
    // Constructor & Destructor
    RIPTetris();
    ~RIPTetris();

    // Core Functions
    void gameLoop(direction input);           // Runs the game loop

    void genShape();           // Generates a new Tetrimino

    void checkInput(direction input);         // Handles player input

    void alterShape(int mode); // Moves or rotates shape
    void getRotatedCoordinates(int output[4][2]);
    bool validPosition(int coords[4][2]);

    void scanAndClearGrid();   // Clears rows and updates score

    void getDisplayMatrix(int outMatrix[GRID_HEIGHT][GRID_WIDTH]);

    // Display
    int gameMatrix[CHECKING_HEIGHT][GRID_WIDTH];
    int score = 0;

private:

    // Game State
    int shape[4][2];       // Relative positions of blocks
    int ShapeCoordinates[4][2]; // Absolute positions of blocks
    bool active = false;           // Whether the shape is currently active

    bool end = false;

    // Timing Variables
    unsigned long lastInputTime = 0;
    unsigned long inputInterval = 100;
    unsigned long prev = 0;
    unsigned long interval = 500;
    bool previousRotateState = LOW;
};

#endif

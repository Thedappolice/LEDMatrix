#ifndef RIPTETRIS_H
#define RIPTETRIS_H

#include <Arduino.h>
#include <LEDMatrix.h>

// Pin Definitions for Controls
#define ROTATE_PIN 32
#define LEFT_OR_RIGHT_PIN 23
#define UP_OR_DOWN_PIN 22

// Grid Constants
#define GRID_WIDTH 8
#define GRID_HEIGHT (GRID_WIDTH * 2)
#define CHECKING_HEIGHT (GRID_HEIGHT + 2)

// Class Definition
class RIPTetris
{
public:
    // Constructor & Destructor
    RIPTetris(LEDMatrix<8, 8> &topDisplay, LEDMatrix<8, 8> &botDisplay);
    ~RIPTetris();

    // Core Functions
    void gameLoop();           // Runs the game loop
    void checkInput();         // Handles player input
    void alterShape(int mode); // Moves or rotates shape
    void genShape();           // Generates a new Tetrimino
    void scanAndClearGrid();   // Clears rows and updates score
    void gatherAndDisplay();   // Updates LED matrix
    void showEndAnimation();   // Displays game over animation
    void sendScore(int score); // Sends score over Serial

private:
    // Display
    LEDMatrix<8, 8> *topLM;
    LEDMatrix<8, 8> *botLM;
    int displayMatrix[8][8];

    // Game State
    int shape[4][2];       // Relative positions of blocks
    int coordinates[4][2]; // Absolute positions of blocks
    bool active;           // Whether the shape is currently active
    int score = 0;
    bool end = false;
    bool ended = false;

    // Timing Variables
    unsigned long lastInputTime = 0;
    unsigned long inputInterval = 100;
    unsigned long prev = 0;
    unsigned long interval = 500;
    bool previousRotateState = LOW;
};

#endif

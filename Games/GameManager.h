#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <Arduino.h>
#include <LEDMatrix.h>
#include "Joystick_snake.h"
#include "RIPTetris.h"

// Game state enum
enum Game
{
    Empty,
    Snake,
    Tetris
};

// Input state enum
enum inputState
{
    up,
    down,
    left,
    right,
    neutral
};

class GameManager
{
public:
    GameManager(LEDMatrix<8, 8> &LMtop, LEDMatrix<8, 8> &LMbot);
    ~GameManager();

    void update();
    void checkInput();
    void showMenu();
    void selection();
    void resetGame(); // New function to return to menu

private:
    LEDMatrix<8, 8> *topLM;
    LEDMatrix<8, 8> *botLM;

    Game game = Empty;
    Joystick_snake *snake = nullptr;
    RIPTetris *tetris = nullptr;

    bool cursor = true;
    int topDisplay[8][8] = {{0}};
    int botDisplay[8][8] = {{0}};

    inputState left_right = neutral;
    inputState up_down = neutral;
    bool rotate = false;
    bool prevRotateState = false; // Prevent accidental multiple selections

    bool resetTrigger = false; // Prevents continuous reset triggering
};

#endif

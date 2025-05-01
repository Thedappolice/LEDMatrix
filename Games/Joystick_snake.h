#ifndef JOYSTICK_SNAKE_H
#define JOYSTICK_SNAKE_H

#include <Arduino.h>
#include <LEDMatrix.h>

#define GRID_SIZE 8   // 8x8 Grid
#define MAX_LENGTH 64 // Max snake length

class Joystick_snake
{
public:
    Joystick_snake(LEDMatrix<GRID_SIZE, GRID_SIZE> &displayMatrix);
    ~Joystick_snake();

    void gameLoop();
    void checkInput();
    void generateFood();
    void updateSnake();
    void displayGame();
    void endGame();
    void showSymbol(char input, unsigned long duration = 0);

private:
    LEDMatrix<GRID_SIZE, GRID_SIZE> *LM;

    int length = 3;
    int body[MAX_LENGTH][2] = {{4, 4}, {5, 4}, {6, 4}};
    int head[2] = {4, 4};
    bool foodExists = false;
    int memory[GRID_SIZE][GRID_SIZE] = {0};

    char direction = 'u';
    bool end = false;
    bool win = false;

    unsigned long interval = 500;
};

#endif

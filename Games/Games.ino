#include <Arduino.h>
#include <LEDMatrix.h>
#include "GameManager.h"

// Pin configurations
int posPintop[] = {33, 38, 41, 36, 19, 13, 18, 15};
int negPintop[] = {37, 17, 16, 34, 14, 35, 39, 40};
int posPinbot[] = {9, 4, 28, 6, 10, 27, 11, 25};
int negPinbot[] = {5, 12, 24, 8, 26, 7, 3, 2};

// LED Matrices
LEDMatrix<8, 8> LMtop(posPintop, negPintop);
LEDMatrix<8, 8> LMbot(posPinbot, negPintop);

// Game Manager Instance
GameManager gameManager(LMtop, LMbot);

void setup()
{
    pinMode(ROTATE_PIN, INPUT_PULLUP);
}

void loop()
{
    gameManager.update();
}

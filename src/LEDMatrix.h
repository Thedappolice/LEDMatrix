#ifndef LEDMatrix_H
#define LEDMatrix_H

#include <Arduino.h>

class LEDMatrix
{
public:
    LEDMatrix(int posPins[], int posPinSize, int negPins[], int negPinSize); // constructor

    void turnOn(int xCol, int yRow, int delayTime = 2); // turn on x-Col's, y-Row's LED

    void customCol(int array[8], int xCol, int shift = 0, int delayTime = 2); // turn on x-Col by given array, shiftable by +/- n
    void customRow(int array[8], int yRow, int shift = 0, int delayTime = 2); // turn on y-Row by given array, shiftable by +/- n

    void OnRow(int yRow, int delayTime = 2); // turn on entire y-Row
    void OnCol(int xCol, int delayTime = 2); // turn on entire x-Col

    void Test(); // check all possible LED

    void Symbol(int UserMatrix[][8], unsigned long showTime = 1000); // display custom symbol

private:
    int Col;
    int Row;

    int *Pins;
    int NumPins;

    void Clear(int delayTime = 2,int n_Row = -1); // clears the display, delayable

    int limitingCols(int xCol); // limits x inputs
    int limitingRows(int yRow); // limits y inputs

    int *OutputCol;
    int *OutputRow;
};

#endif

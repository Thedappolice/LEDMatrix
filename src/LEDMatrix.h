#ifndef LEDMatrix_H
#define LEDMatrix_H

#include <Arduino.h>

class LEDMatrix
{
public:
    LEDMatrix(int posPins[], int posPinSize, int negPins[], int negPinSize);

    void turnOn(int xCol, int yRow);

    void customCol(int array[8],int xCol, int shift = 0);
    void customRow(int array[8],int yRow,  int shift = 0);

    void OnRow(int yRow);
    void OnCol(int xCol);

    void Test();

    void Symbol(int UserMatrix[][8]);

private:
    int Col;
    int Row;

    int *Pins;
    int NumPins;

    void Clear();

    int limitingCols(int xCol);
    int limitingRows(int yRow);

    int *OutputCol;
    int *OutputRow;
};

#endif

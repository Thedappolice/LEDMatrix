#ifndef LEDMatrix_H
#define LEDMatrix_H

#include <Arduino.h>

class LEDMatrix
{
public:
    LEDMatrix(int posPins[], int posPinSize, int negPins[], int negPinSize);

    void turnOn(int xCol, int yRow);

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

    int **Matrix;

    void setupMatrix(int UserMatrix[][8]);

};

#endif

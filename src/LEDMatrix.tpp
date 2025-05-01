#include "LEDMatrix.h"

// Constructor: Initializes LED matrix pins
template <size_t ColSize, size_t RowSize>
LEDMatrix<ColSize, RowSize>::LEDMatrix(int posPins[], int negPins[])
{
  Pins = new int[NumPins];
  for (size_t i = 0; i < ColSize; i++)
    Pins[i] = posPins[i];
  for (size_t i = 0; i < RowSize; i++)
    Pins[ColSize + i] = negPins[i];

  for (size_t i = 0; i < NumPins; i++)
    pinMode(Pins[i], OUTPUT);

  OutputArray = new int[max(ColSize, RowSize)];
  pinReq = new int[NumPins];
  pinPrev = new int[NumPins];
}

// Turn on a specific LED
template <size_t ColSize, size_t RowSize>
void LEDMatrix<ColSize, RowSize>::turnOn(int Col, int Row, int delayTime)
{
  for (size_t i = 0; i < RowSize; i++)
    pinReq[i] = (i == limitingGrid(Row, 1)) ? 1 : 0;

  for (size_t i = 0; i < ColSize; i++)
    pinReq[i + RowSize] = (i == limitingGrid(Col, 0)) ? 0 : 1;

  setPins();
  delay(delayTime);
}

// Turn on an entire column
template <size_t ColSize, size_t RowSize>
void LEDMatrix<ColSize, RowSize>::OnCol(int Col, int delayTime)
{
  for (size_t i = 0; i < RowSize; i++)
    pinReq[i] = 1;
  for (size_t i = 0; i < ColSize; i++)
    pinReq[i + RowSize] = (i == limitingGrid(Col, 0)) ? 0 : 1;

  setPins();
  delay(delayTime);
}

// Turn on an entire row
template <size_t ColSize, size_t RowSize>
void LEDMatrix<ColSize, RowSize>::OnRow(int Row, int delayTime)
{
  for (size_t i = 0; i < RowSize; i++)
    pinReq[i] = (i == limitingGrid(Row, 1)) ? 1 : 0;

  for (size_t i = 0; i < ColSize; i++)
    pinReq[i + RowSize] = 0;

  setPins();
  delay(delayTime);
}

// Custom column display with shift support
template <size_t ColSize, size_t RowSize>
void LEDMatrix<ColSize, RowSize>::customCol(int array[], int Col, int shift, int delayTime)
{
  adjustShift(shift, array, 0);
  for (size_t i = 0; i < RowSize; i++)
    pinReq[i] = (OutputArray[i]) ? 1 : 0;

  for (size_t i = 0; i < ColSize; i++)
    pinReq[i + RowSize] = (i == limitingGrid(Col, 0)) ? 0 : 1;

  setPins();
  delay(delayTime);
}

// Custom row display with shift support
template <size_t ColSize, size_t RowSize>
void LEDMatrix<ColSize, RowSize>::customRow(int array[], int Row, int shift, int delayTime)
{
  adjustShift(shift, array, 1);
  for (size_t i = 0; i < RowSize; i++)
    pinReq[i] = (i == limitingGrid(Row, 1)) ? 1 : 0;

  for (size_t i = 0; i < ColSize; i++)
    pinReq[i + RowSize] = (OutputArray[i]) ? 0 : 1;

  setPins();
  delay(delayTime);
}

// Test mode: Cycles through rows and columns

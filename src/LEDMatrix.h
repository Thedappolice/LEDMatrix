#ifndef LEDMatrix_H
#define LEDMatrix_H

#include <Arduino.h>

template <size_t ColSize, size_t RowSize>
class LEDMatrix
{
public:
  // Constructor
  LEDMatrix(int posPins[], int negPins[]);

  // Turn on x-Col's, y-Row's LED
  virtual void turnOn(int Col, int Row, int delayTime = 2);

  // Turn on entire y-Row
  virtual void OnRow(int Row, int delayTime = 2);

  // Turn on entire x-Col
  virtual void OnCol(int Col, int delayTime = 2);

  // Turn on x-Col by given array, shiftable by +/- n
  virtual void customCol(int array[], int Col, int shift = 0, int delayTime = 2);

  // Turn on y-Row by given array, shiftable by +/- n
  virtual void customRow(int array[], int Row, int shift = 0, int delayTime = 2);

  // Check all possible LED
  virtual void Test(int delayTime = 100);

  // Display custom symbol
  virtual void Symbol(int UserMatrix[RowSize][ColSize], unsigned long showTime = 1000);

private:
  size_t NumPins = ColSize + RowSize;

  int *Pins; // Pointer for dynamically allocated pins

  int *pinReq;  // running the pins
  int *pinPrev; // pin state b4

  void setPins();

protected:          // let inheritance access the array
  int *OutputArray; // pointer to array for Output

  // shifting function
  void adjustShift(int shfit, int array[], bool axis = 0);

  // Limits inputs to the grid
  size_t limitingGrid(int value, bool axis = 0);
};

#include "LEDMatrix.tpp"

#endif

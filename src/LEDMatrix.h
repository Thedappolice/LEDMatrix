#ifndef LEDMatrix_H
#define LEDMatrix_H

#include <Arduino.h>

template <size_t ColSize, size_t RowSize>
class LEDMatrix
{
public:
  LEDMatrix(int posPins[], int negPins[]); // Constructor

  virtual void turnOn(int xCol, int yRow, int delayTime = 2);                           // Turn on x-Col's, y-Row's LED
  virtual void customCol(int array[8], size_t xCol, int shift = 0, int delayTime = 2);  // Turn on x-Col by given array, shiftable by +/- n
  virtual void customRow(int array[8], size_t yRow, int shift = 0, int delayTime = 2);  // Turn on y-Row by given array, shiftable by +/- n
  virtual void OnRow(size_t yRow, int delayTime = 2);                                   // Turn on entire y-Row
  virtual void OnCol(size_t xCol, int delayTime = 2);                                   // Turn on entire x-Col
  virtual void Test();                                                                  // Check all possible LED
  virtual void Symbol(int UserMatrix[RowSize][ColSize], unsigned long showTime = 1000); // Display custom symbol

private:
  size_t Col; // Use size_t for array sizes
  size_t Row; // Use size_t for array sizes
  size_t NumPins;

  int *Pins;      // Pointer for dynamically allocated pins
  int *OutputCol; // Pointer for dynamically allocated column output
  int *OutputRow; // Pointer for dynamically allocated row output

  void Clear(int delayTime = 2, int n_Row = -1); // Clears the display, delayable
  size_t limitingCols(size_t xCol);              // Limits x inputs
  size_t limitingRows(size_t yRow);              // Limits y inputs
};

#include "LEDMatrix.tpp"

#endif

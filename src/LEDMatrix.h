#ifndef LEDMatrix_H
#define LEDMatrix_H

#include <Arduino.h>

/**
 * @class LEDMatrix
 * @brief A generic LED matrix controller for ColSize x RowSize grids.
 * @tparam ColSize Number of columns in the LED matrix.
 * @tparam RowSize Number of rows in the LED matrix.
 */
template <size_t ColSize, size_t RowSize>
class LEDMatrix
{
public:
  /**
   * @brief Constructor to initialize the LED matrix pins.
   * @param posPins Array of positive pin numbers.
   * @param negPins Array of negative pin numbers.
   */
  LEDMatrix(int posPins[], int negPins[]);

  /**
   * @brief Turn on a specific LED at (Col, Row).
   * @param Col Column index.
   * @param Row Row index.
   * @param delayTime Time in milliseconds to hold the LED state.
   */
  virtual void turnOn(int Col, int Row, int delayTime = 2);

  /**
   * @brief Turn on an entire row of LEDs.
   * @param Row Row index.
   * @param delayTime Time in milliseconds to hold the row state.
   */
  virtual void OnRow(int Row, int delayTime = 2);

  /**
   * @brief Turn on an entire column of LEDs.
   * @param Col Column index.
   * @param delayTime Time in milliseconds to hold the column state.
   */
  virtual void OnCol(int Col, int delayTime = 2);

  /**
   * @brief Custom column lighting with optional shifting.
   * @param array Array defining the custom pattern for the column.
   * @param Col Column index.
   * @param shift Number of positions to shift the pattern (+/-).
   * @param delayTime Time in milliseconds to hold the column state.
   */
  virtual void customCol(int array[], int Col, int shift = 0, int delayTime = 2);

  /**
   * @brief Custom row lighting with optional shifting.
   * @param array Array defining the custom pattern for the row.
   * @param Row Row index.
   * @param shift Number of positions to shift the pattern (+/-).
   * @param delayTime Time in milliseconds to hold the row state.
   */
  virtual void customRow(int array[], int Row, int shift = 0, int delayTime = 2);

  /**
   * @brief Test the LED matrix by sequentially lighting up all LEDs.
   * @param delayTime Time in milliseconds between each LED activation.
   */
  virtual void Test(int delayTime = 100);

  /**
   * @brief Display a custom symbol on the matrix for a specified duration.
   * @param UserMatrix 2D array representing the symbol pattern.
   * @param showTime Time in milliseconds to display the symbol.
   */
  virtual void Symbol(int UserMatrix[RowSize][ColSize], unsigned long showTime = 1000);

private:
  /**
   * @brief Constrain a value to stay within the grid bounds.
   * @param axis 0 for columns, 1 for rows.
   * @param value The value to constrain.
   * @return The constrained value.
   */
  size_t limitingGrid(bool axis, int value);

  /**
   * @brief Adjust and shift a given array based on the shift value.
   * @param shift Number of positions to shift the array.
   * @param array The array to shift.
   * @param axis 0 for columns, 1 for rows.
   */
  void adjustShift(int shift, int array[], bool axis);

  /**
   * @brief Update pin states based on the current pin request.
   */
  void setPins();

  size_t NumPins = ColSize + RowSize;     ///< Total number of pins.
  int Pins[NumPins];                      ///< Array of all pin numbers.
  int pinReq[NumPins];                    ///< Requested pin states.
  int pinPrev[NumPins];                   ///< Previous pin states.
  int OutputArray[max(ColSize, RowSize)]; ///< Array for output patterns.
};

#include "LEDMatrix.tpp"

#endif

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
template <size_t ColSize, size_t RowSize>
void LEDMatrix<ColSize, RowSize>::Test(int delayTime)
{
  // uses raw code
  // Column check
  for (size_t i = 0; i < NumPins; i++)
  {
    digitalWrite(Pins[i], LOW);
  }
  for (size_t i = 0; i < ColSize; i++)
  {
    digitalWrite(Pins[i], HIGH);
    delay(delayTime);
  }

  // RowSize check
  for (size_t i = 0; i < NumPins; i++)
  {
    digitalWrite(Pins[i], HIGH);
  }
  for (size_t i = 0; i < RowSize; i++)
  {
    digitalWrite(Pins[ColSize + i], LOW);
    delay(delayTime);
  }
}

/**
 * @brief Display a custom symbol on the LED matrix.
 */
template <size_t ColSize, size_t RowSize>
void LEDMatrix<ColSize, RowSize>::Symbol(int UserMatrix[RowSize][ColSize], unsigned long showTime, bool clear)
{
  int TransformedMatrix[ColSize][RowSize] = {0};

  // Apply transformations to the matrix (reflection and rotation)
  for (size_t i = 0; i < RowSize; i++)
    for (size_t j = 0; j < ColSize; j++)
      TransformedMatrix[ColSize - 1 - j][i] = UserMatrix[i][ColSize - 1 - j];

  unsigned long before = millis();
  while (millis() - before < showTime)
  {
    for (size_t i = 0; i < RowSize; i++)
    {
      memcpy(OutputArray, TransformedMatrix[i], ColSize * sizeof(int));
      customCol(OutputArray, i);
      setPins();
      delay(1);
    }
  }
  if (clear)
  {
    for (size_t i = 0; i < NumPins; i++)
    {
      digitalWrite(Pins[i], LOW);
    }
  }
}

// Remaining private methods...
/**
 * @brief Constrain a value to the valid range of columns or rows.
 *
 * @param axis 0 for columns, 1 for rows.
 * @param value The value to constrain.
 * @return The constrained value as a valid size_t index.
 */
template <size_t ColSize, size_t RowSize>
size_t LEDMatrix<ColSize, RowSize>::limitingGrid(int value, bool axis)
{
  // Determine the maximum range based on axis (ColSize or RowSize)
  int check = (!axis) ? ColSize : RowSize;

  // Constrain the value to be within 0 and the maximum index
  return (size_t)constrain(value, 0, check - 1);
}

/**
 * @brief Adjust and shift an array based on the provided shift value.
 *
 * @param shift Number of positions to shift. Positive for left/up, negative for right/down.
 * @param array Input array to be shifted.
 * @param axis 0 for columns, 1 for rows.
 */
template <size_t ColSize, size_t RowSize>
void LEDMatrix<ColSize, RowSize>::adjustShift(int shift, int array[], bool axis)
{
  // Determine the maximum length based on axis (ColSize or RowSize)
  int check = (!axis) ? ColSize : RowSize;

  // Constrain the shift value to prevent out-of-bound shifts
  shift = constrain(shift, -check, check);

  if (shift > 0) // Shift upwards/leftwards
  {
    for (int i = 0; i < shift; i++) // Fill the first 'shift' elements with 0
      OutputArray[i] = 0;

    for (int i = 0; i < (check - shift); i++) // Copy the rest of the array
      OutputArray[i + shift] = array[i];
  }
  else if (shift < 0) // Shift downwards/rightwards
  {
    for (int i = 0; i < (check + shift); i++) // Skip initial 'shift' elements
      OutputArray[i] = array[i - shift];

    for (int i = (check + shift); i < check; i++) // Fill the remaining elements with 0
      OutputArray[i] = 0;
  }
  else // No shift
  {
    memcpy(OutputArray, array, check * sizeof(int)); // Copy the input array directly
  }
}

/**
 * @brief Update the pin states based on the current pin request.
 */
template <size_t ColSize, size_t RowSize>
void LEDMatrix<ColSize, RowSize>::setPins()
{
  for (size_t i = 0; i < NumPins; i++)
  {
    // Only update the pin if the requested state differs from the previous state
    if (pinPrev[i] != pinReq[i])
    {
      pinPrev[i] = pinReq[i];                        // Update the previous state
      digitalWrite(Pins[i], pinReq[i] ? HIGH : LOW); // Write the new state to the pin
    }
  }
}
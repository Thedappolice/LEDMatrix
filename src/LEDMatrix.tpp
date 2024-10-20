#include "LEDMatrix.h"

// Implementations inside the header
template <size_t ColSize, size_t RowSize>
LEDMatrix<ColSize, RowSize>::LEDMatrix(int posPins[], int negPins[])
{
  Pins = new int[NumPins];             // record all pins in order from pos to neg
  for (size_t i = 0; i < ColSize; i++) // record all positive pins
  {
    Pins[i] = posPins[i];
  }
  for (size_t i = 0; i < RowSize; i++) // record all negative pins
  {
    Pins[ColSize + i] = negPins[i];
  }

  for (size_t i = 0; i < NumPins; i++) // set all pins to output
  {
    pinMode(Pins[i], OUTPUT);
  }

  OutputArray = new int[max(ColSize, RowSize)]; // output row placeholder

  pinReq = new int[NumPins];  // for digitalWrite() requests
  pinPrev = new int[NumPins]; // record the previous pin state
}

template <size_t ColSize, size_t RowSize>
void LEDMatrix<ColSize, RowSize>::turnOn(int Col, int Row, int delayTime)
{
  for (size_t i = 0; i < RowSize; i++) // recording what arrangement of positive pins
  {
    pinReq[i] = (i == limitingGrid(0, Row)) ? 1 : 0;
  }

  for (size_t i = 0; i < ColSize; i++) // recording what arrangement of negative pins
  {
    pinReq[i + RowSize] = (i == limitingGrid(0, Col)) ? 0 : 1;
  }

  setPins(); // run the pins

  delay(delayTime);
}

template <size_t ColSize, size_t RowSize>
void LEDMatrix<ColSize, RowSize>::OnCol(int Col, int delayTime)
{
  for (size_t i = 0; i < RowSize; i++) // recording what arrangement of positive pins
  {
    pinReq[i] = 1;
  }

  for (size_t i = 0; i < ColSize; i++) // recording what arrangement of negative pins
  {
    pinReq[i + RowSize] = (i == limitingGrid(0, Col)) ? 0 : 1;
  }

  setPins(); // run the pins

  delay(delayTime);
}

template <size_t ColSize, size_t RowSize>
void LEDMatrix<ColSize, RowSize>::OnRow(int Row, int delayTime)
{
  for (size_t i = 0; i < RowSize; i++) // recording what arrangement of positive pins
  {
    pinReq[i] = (i == limitingGrid(1, Row)) ? 1 : 0;
  }

  for (size_t i = 0; i < ColSize; i++) // recording what arrangement of negative pins
  {
    pinReq[i + RowSize] = 0;
  }

  setPins(); // run the pins

  delay(delayTime);
}

template <size_t ColSize, size_t RowSize>
void LEDMatrix<ColSize, RowSize>::customCol(int array[], int Col, int shift, int delayTime)
{
  adjustShift(shift, array, 0); // shifting function

  for (size_t i = 0; i < RowSize; i++) // recording what arrangement of positive pins
  {
    pinReq[i] = (OutputArray[i]) ? 1 : 0;
  }

  for (size_t i = 0; i < ColSize; i++) // recording what arrangement of negative pins
  {
    pinReq[i + RowSize] = (i == limitingGrid(0, Col)) ? 0 : 1;
  }

  setPins(); // run the pins

  delay(delayTime);
}

template <size_t ColSize, size_t RowSize>
void LEDMatrix<ColSize, RowSize>::customRow(int array[], int Row, int shift, int delayTime)
{
  adjustShift(shift, array, 1);

  for (size_t i = 0; i < RowSize; i++) // recording what arrangement of positive pins
  {
    pinReq[i] = (i == limitingGrid(1, Row)) ? 1 : 0;
  }

  for (size_t i = 0; i < ColSize; i++) // recording what arrangement of negative pins
  {
    pinReq[i + RowSize] = (OutputArray[i]) ? 0 : 1;
  }

  setPins(); // run the pins

  delay(delayTime);
}


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

template <size_t ColSize, size_t RowSize>
void LEDMatrix<ColSize, RowSize>::Symbol(int UserMatrix[RowSize][ColSize], unsigned long showTime)
{
  unsigned long before = millis();
  while (millis() - before < showTime)
  {
    for (size_t i = 0; i < RowSize; i++) // repeating for each RowSize
    {
      memcpy(OutputArray, UserMatrix[i], ColSize * sizeof(int)); // copy the array directly

      for (size_t j = 0; j < RowSize; j++) // recording what arrangement of positive pins
      {
        pinReq[i] = (OutputArray[j]) ? 1 : 0;
      }

      for (size_t j = 0; j < ColSize; j++) // recording what arrangement of negative pins
      {
        pinReq[j + RowSize] = 1;
      }
      pinReq[i + RowSize] = 0;

      setPins(); // run the pins

      delay(2);
    }
  }
}

// private:

template <size_t ColSize, size_t RowSize>
size_t LEDMatrix<ColSize, RowSize>::limitingGrid(bool axis, int value)
{
  int check = (!axis) ? ColSize : RowSize;
  return (size_t)constrain(value, 0, check - 1); // use Arduino's built-in constrain() function
}

template <size_t ColSize, size_t RowSize>
void LEDMatrix<ColSize, RowSize>::adjustShift(int shift, int array[], bool axis)
{
  int check = (!axis) ? ColSize : RowSize;
  if (shift > 0) // shift upwards/left
  {
    for (int i = 0; i < shift; i++) // place 0 at the front
    {
      OutputArray[i] = 0;
    }
    for (int i = 0; i < (check - shift); i++) // for the rest of the spaces, copy back given array
    {
      OutputArray[i + shift] = array[i];
    }
  }
  else if (shift < 0) // shift downwards/right
  {
    for (int i = 0; i < (check + shift); i++) // skip 'shift' amounts of the given array, then copy the rest
    {
      OutputArray[i] = array[i - shift]; // copy valid elements
    }

    for (int i = (check + shift); i < check; i++) // fill zeros in remaining space
    {
      OutputArray[i] = 0; // no out-of-bounds issue here
    }
  }
  else if (shift == 0)
  {
    memcpy(OutputArray, array, check * sizeof(int)); // copy the array directly
  }
}

template <size_t ColSize, size_t RowSize>
void LEDMatrix<ColSize, RowSize>::setPins()
{
  for (size_t i = 0; i < NumPins; i++)
  {
    // Only update the pin if the requested state is different from the previous state
    if (pinPrev[i] != pinReq[i])
    {
      pinPrev[i] = pinReq[i];                        // Update the previous state
      digitalWrite(Pins[i], pinReq[i] ? HIGH : LOW); // Perform the write in the same loop
    }
  }
}

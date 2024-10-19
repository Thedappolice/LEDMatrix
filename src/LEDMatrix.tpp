#include "LEDMatrix.h"

// Implementations inside the header
template <size_t ColSize, size_t RowSize>
LEDMatrix<ColSize, RowSize>::LEDMatrix(int posPins[], int negPins[])
{
  this->Col = ColSize;
  this->Row = RowSize;
  this->NumPins = Col + Row;

  Pins = new int[NumPins];
  OutputCol = new int[Col];
  OutputRow = new int[Row];

  for (size_t i = 0; i < Col; i++)
  {
    Pins[i] = posPins[i];
  }
  for (size_t i = 0; i < Row; i++)
  {
    Pins[Col + i] = negPins[i];
  }
  for (size_t i = 0; i < NumPins; i++)
  {
    pinMode(Pins[i], OUTPUT);
  }
}

template <size_t ColSize, size_t RowSize>
void LEDMatrix<ColSize, RowSize>::turnOn(int xCol, int yRow, int delayTime)
{
  digitalWrite(Pins[limitingGrid(0, xCol)], HIGH);
  for (size_t i = 0; i < Row; i++)
  {
    if (i == limitingGrid(1, yRow))
    {
      digitalWrite(Pins[Col + i], LOW);
    }
    else
    {
      digitalWrite(Pins[Col + i], HIGH);
    }
  }
  Clear(delayTime);
}

template <size_t ColSize, size_t RowSize>
void LEDMatrix<ColSize, RowSize>::customCol(int array[8], int xCol, int shift, int delayTime)
{
  if (shift > 0)
  {
    for (int i = 0; i < shift; i++)
    {
      OutputCol[i] = 0;
    }
    for (int i = 0; i < (8 - shift); i++)
    {
      OutputCol[i + shift] = array[i];
    }
  }
  else if (shift < 0)
  {
    for (int i = 0; i < (8 + shift); i++)
    {
      if (i - shift < 8)
      {
        OutputCol[i] = array[i - shift];
      }
    }
    for (int i = 0; i < -shift; i++)
    {
      OutputCol[8 + shift + i] = 0;
    }
  }
  else if (shift == 0)
  {
    for (int i = 0; i < 8; i++)
    {
      OutputCol[i] = array[i];
    }
  }

  digitalWrite(Pins[limitingGrid(0, xCol)], HIGH);
  for (size_t i = 0; i < Row; i++)
  {
    if (OutputCol[i] == 1)
    {
      digitalWrite(Pins[Col + i], LOW);
    }
    else
    {
      digitalWrite(Pins[Col + i], HIGH);
    }
  }

  Clear(delayTime);
}

template <size_t ColSize, size_t RowSize>
void LEDMatrix<ColSize, RowSize>::customRow(int array[8], int yRow, int shift, int delayTime)
{
  if (shift > 0)
  {
    for (int i = 0; i < shift; i++)
    {
      OutputRow[i] = 0;
    }
    for (int i = 0; i < (8 - shift); i++)
    {
      OutputRow[i + shift] = array[i];
    }
  }
  else if (shift < 0)
  {
    for (int i = 0; i < (8 + shift); i++)
    {
      if (i - shift < 8)
      {
        OutputRow[i] = array[i - shift];
      }
    }
    for (int i = 0; i < -shift; i++)
    {
      OutputRow[8 + shift + i] = 0;
    }
  }
  else if (shift == 0)
  {
    for (int i = 0; i < 8; i++)
    {
      OutputRow[i] = array[i];
    }
  }

  for (size_t i = 0; i < Row; i++)
  {
    if (i == limitingGrid(1, yRow))
    {
      digitalWrite(Pins[Col + i], LOW);
    }
    else
    {
      digitalWrite(Pins[Col + i], HIGH);
    }
  }
  for (size_t i = 0; i < Col; i++)
  {
    if (OutputRow[i] == 1)
    {
      digitalWrite(Pins[i], HIGH);
    }
    else
    {
      digitalWrite(Pins[i], LOW);
    }
  }
  Clear(delayTime);
}

template <size_t ColSize, size_t RowSize>
void LEDMatrix<ColSize, RowSize>::OnCol(int xCol, int delayTime)
{
  digitalWrite(Pins[limitingGrid(0, xCol)], HIGH);
  for (size_t i = 0; i < Row; i++)
  {
    digitalWrite(Pins[Col + i], LOW);
  }
  Clear(delayTime);
}

template <size_t ColSize, size_t RowSize>
void LEDMatrix<ColSize, RowSize>::OnRow(int yRow, int delayTime)
{
  for (size_t i = 0; i < Col; i++)
  {
    digitalWrite(Pins[i], HIGH);
  }
  for (size_t i = 0; i < Row; i++) // Use size_t instead of int
  {
    if (i == limitingGrid(1, yRow))
    {
      digitalWrite(Pins[Col + i], LOW);
    }
    else
    {
      digitalWrite(Pins[Col + i], HIGH);
    }
  }
  Clear(delayTime);
}

template <size_t ColSize, size_t RowSize>
void LEDMatrix<ColSize, RowSize>::Test()
{
  // Column check
  for (size_t i = 0; i < NumPins; i++)
  {
    digitalWrite(Pins[i], LOW);
  }
  for (size_t i = 0; i < Col; i++)
  {
    digitalWrite(Pins[i], HIGH);
    delay(100);
  }

  // Row check
  for (size_t i = 0; i < NumPins; i++)
  {
    digitalWrite(Pins[i], HIGH);
  }
  for (size_t i = 0; i < Row; i++)
  {
    digitalWrite(Pins[Col + i], LOW);
    delay(100);
  }
}

// template <size_t ColSize, size_t RowSize>
// void LEDMatrix<ColSize, RowSize>::Symbol(int UserMatrix[RowSize][ColSize], unsigned long showTime)
// {
//   unsigned long before = millis();
//   while (millis() - before < showTime)
//   {
//     for (size_t i = 0; i < Row; i++) // repeating for each Row
//     {
//       for (size_t j = 0; j < Col; j++) // repeating for each entity of array

//         if (UserMatrix[j][i] == 1)
//         {
//           digitalWrite(Pins[j], HIGH); // pulling pin HIGH to light LED
//         }
//         else
//         {
//           digitalWrite(Pins[j], LOW); // pulling pin LOW to dim LED
//         }

//       // pulling the i-th Row Pin LOW
//       digitalWrite(Pins[i + Col], LOW);

//       Clear(2, i); // Clearing up
//     }
//   }
// }
template <size_t ColSize, size_t RowSize>
void LEDMatrix<ColSize, RowSize>::Symbol(int UserMatrix[RowSize][ColSize], unsigned long showTime)
{
  uint8_t Rows[Col];
  for (size_t i = 0; i < Row; i++)
  {
    for (size_t j = 0; i < Col; i++)
    {
      if (UserMatrix[i][j] == 1)
      {
        Rows[i] = 0b000000 |= (1 << j);
      }
    }
  }

  unsigned long before = millis();
  while (millis() - before < showTime)
  {
    for (size_t i = 0; i < Row; i++) // repeating for each Row
    {
      for (size_t j = 0; j < Col; j++) // repeating for each entity of array

        if (UserMatrix[j][i] == 1)
        {
          digitalWrite(Pins[j], HIGH); // pulling pin HIGH to light LED
        }
        else
        {
          digitalWrite(Pins[j], LOW); // pulling pin LOW to dim LED
        }

      // pulling the i-th Row Pin LOW
      digitalWrite(Pins[i + Col], LOW);

      Clear(2, i); // Clearing up
    }
  }
}

// private:

template <size_t ColSize, size_t RowSize>
size_t LEDMatrix<ColSize, RowSize>::limitingGrid(bool axis, int value)
{
  int check = (!axis) ? Col : Row;
  int limited = value;
  if (value >= check)
  {
    limited = check - 1;
  }
  else if (value < 0)
  {
    limited = 0;
  }
  return (size_t)limited;
}

template <size_t ColSize, size_t RowSize>
void LEDMatrix<ColSize, RowSize>::Clear(int delayTime, int n_Row)
{
  delay(delayTime);
  if (n_Row != -1)
  {
    digitalWrite(Pins[Col + n_Row], HIGH);
  }
  else
  {
    for (size_t i = 0; i < Row; i++)
    {
      digitalWrite(Pins[Col + i], HIGH);
    }
  }
  for (size_t i = 0; i < Col; i++)
  {
    digitalWrite(Pins[i], LOW);
  }
}
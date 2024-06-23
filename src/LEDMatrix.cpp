#include "LEDMatrix.h"

// public:
LEDMatrix::LEDMatrix(int posPins[], int posPinSize, int negPins[], int negPinSize)
{
  Col = posPinSize;
  Row = negPinSize;
  NumPins = Col + Row;
  Pins = new int[NumPins];

  OutputCol = new int[Col];
  OutputRow = new int[Row];

  for (int i = 0; i < Col; i++)
  {
    Pins[i] = posPins[i];
  }
  for (int i = 0; i < Row; i++)
  {
    Pins[Col + i] = negPins[i];
  }

  for (int i = 0; i < NumPins; i++)
  {
    pinMode(Pins[i], OUTPUT);
  }
}

void LEDMatrix::turnOn(int xCol, int yRow)
{
  digitalWrite(Pins[limitingCols(xCol)], HIGH);
  for (int i = 0; i < Row; i++)
  {
    if (i == limitingRows(yRow))
    {
      digitalWrite(Pins[Col + i], LOW);
    }
    else
    {
      digitalWrite(Pins[Col + i], HIGH);
    }
  }
  Clear();
}

void LEDMatrix::customCol(int array[8], int xCol, int shift)
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

  digitalWrite(Pins[limitingCols(xCol)], HIGH);
  for (int i = 0; i < Row; i++)
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

  Clear(10);
}

void LEDMatrix::customRow(int array[8], int yRow, int shift)
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

  for (int i = 0; i < Row; i++)
  {
    if (i == limitingRows(yRow))
    {
      digitalWrite(Pins[Col + i], LOW);
    }
    else
    {
      digitalWrite(Pins[Col + i], HIGH);
    }
  }
  for (int i = 0; i < Col; i++)
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
  Clear();
}

void LEDMatrix::OnCol(int xCol)
{
  digitalWrite(Pins[limitingCols(xCol)], HIGH);
  for (int i = 0; i < Row; i++)
  {
    digitalWrite(Pins[Col + i], LOW);
  }
  Clear();
}

void LEDMatrix::OnRow(int yRow)
{
  for (int i = 0; i < Col; i++)
  {
    digitalWrite(Pins[i], HIGH);
  }
  for (int i = 0; i < Row; i++)
  {
    if (i == limitingRows(yRow))
    {
      digitalWrite(Pins[Col + i], LOW);
    }
    else
    {
      digitalWrite(Pins[Col + i], HIGH);
    }
  }
  Clear();
}

void LEDMatrix::Test()
{
  // Column check
  for (int i = 0; i < NumPins; i++)
  {
    digitalWrite(Pins[i], LOW);
  }
  for (int i = 0; i < Col; i++)
  {
    digitalWrite(Pins[i], HIGH);
    delay(100);
  }

  // Row check
  for (int i = 0; i < NumPins; i++)
  {
    digitalWrite(Pins[i], HIGH);
  }
  for (int i = 0; i < Row; i++)
  {
    digitalWrite(Pins[Col + i], LOW);
    delay(100);
  }
}

void LEDMatrix::Symbol(int UserMatrix[][8], int delayTime)
{
  for (int i = 0; i < Row; i++) // repeating for each Row
  {
    for (int j = 0; j < Col; j++) // repeating for each entity of array

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

    Clear(i, delayTime); // Clearing up
  }
}

// private:

int LEDMatrix::limitingCols(int xCol)
{
  int limitedx;
  if (xCol >= Col)
  {
    limitedx = Col - 1;
  }
  else if (xCol < 0)
  {
    limitedx = 0;
  }
  else
  {
    limitedx = xCol;
  }
  return limitedx;
}

int LEDMatrix::limitingRows(int yRow)
{
  int limitedy;
  if (yRow >= Row)
  {
    limitedy = Row - 1;
  }
  else if (yRow < 0)
  {
    limitedy = 0;
  }
  else
  {
    limitedy = yRow;
  }
  return limitedy;
}

void LEDMatrix::Clear(int n_Row, int delay)
{
  delay(delay);
  if (n_Row != -1)
  {
    digitalWrite(Pins[Col + n_Row], HIGH);
  }
  else
  {
    for (int i = 0; i < Row; i++)
    {
      digitalWrite(Pins[Col + i], HIGH);
    }
  }
  for (int i = 0; i < Col; i++)
  {
    digitalWrite(Pins[i], LOW);
  }
}
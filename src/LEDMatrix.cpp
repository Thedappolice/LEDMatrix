#include "LEDMatrix.h"

// public:
LEDMatrix::LEDMatrix(int posPins[], int posPinSize, int negPins[], int negPinSize)
{
  Col = posPinSize;
  Row = negPinSize;
  NumPins = Col + Row;
  Pins = new int[NumPins];
  Matrixln = new int[Col];

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

void LEDMatrix::Symbol(int UserMatrix[][8])
{
  for (int i = 0; i < Col; i++) // repeating for each Column
  {
    for (int n = 0; n < Col; n++) // repeating for each entity of array
    {
      Matrixln[n] = UserMatrix[i][n]; // ispecting each entity of array
    }
    digitalWrite(Pins[i], HIGH);  // pulling the i-th Column Pin HIGH
    for (int j = 0; j < Row; j++) // repeating for each Row
    {
      if (Matrixln[j] == 1)
      {
        digitalWrite(Pins[Col + j], LOW); // pulling pin LOW to light LED
      }
      else
      {
        digitalWrite(Pins[Col + j], HIGH); // pulling pin HIGH to dim LED
      }
    }
    Clear(); // Clearing up
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

void LEDMatrix::Clear()
{
  delay(2);
  for (int i = 0; i < NumPins; i++)
  {
    digitalWrite(Pins[i], LOW);
  }
}

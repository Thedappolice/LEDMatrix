#include "LEDMatrix.h"

// public:
LEDMatrix::LEDMatrix(int posPins[], int posPinSize, int negPins[], int negPinSize)
{
  Col = posPinSize;
  Row = negPinSize;
  NumPins = Col + Row;
  Pins = new int[NumPins];

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

  Matrix = new int *[Row];
  for (int i = 0; i < Row; i++)
  {
    Matrix[i] = new int[Col];
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
  setupMatrix(UserMatrix);

  for (int i = 0; i < Col; i++)
  {
    for (int j = 0; j < Row; j++)
    {
      if (Matrix[j][i] == 1)
      {
        turnOn(i, j);
      }
    }
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
  delayMicroseconds(700);
  for (int i = 0; i < NumPins; i++)
  {
    digitalWrite(Pins[i], LOW);
  }
}

void LEDMatrix::setupMatrix(int UserMatrix[][8])
{
  for (int i = 0; i < Col; i++)
  {
    for (int j = 0; j < Row; j++)
    {
      Matrix[j][i] = UserMatrix[i][j];
    }
  }
}

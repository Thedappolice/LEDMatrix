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

void LEDMatrix::turnOn(int xCol, int yRow, int delayTime)
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
  Clear(delayTime);
}

void LEDMatrix::customCol(int array[8], int xCol, int shift, int delayTime)
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

  Clear(delayTime);
}

void LEDMatrix::customRow(int array[8], int yRow, int shift, int delayTime)
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
  Clear(delayTime);
}

void LEDMatrix::OnCol(int xCol, int delayTime)
{
  digitalWrite(Pins[limitingCols(xCol)], HIGH);
  for (int i = 0; i < Row; i++)
  {
    digitalWrite(Pins[Col + i], LOW);
  }
  Clear(delayTime);
}

void LEDMatrix::OnRow(int yRow, int delayTime)
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
  Clear(delayTime);
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

void LEDMatrix::Symbol(int UserMatrix[][8], unsigned long showTime)
{
  unsigned long before = millis();
  while (millis() - before < showTime)
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

      Clear(2, i); // Clearing up
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

void LEDMatrix::Clear(int delayTime, int n_Row)
{
  delay(delayTime);
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

// chipped version
// public :

#define DECODE_MODE_REG 0x09
#define INTENSITY_REG 0x0A
#define SCAN_LIMIT_REG 0x0B
#define SHUTDOWN_REG 0x0C
#define DISPLAY_TEST_REG 0x0F

LEDMatrixChip::LEDMatrixChip(int CS, int CLK, int MOSI)
{
  this->CS = CS;
  this->CLK = CLK;
  this->MOSI = MOSI;

  pinMode(this->CS, OUTPUT);
  pinMode(this->CLK, OUTPUT);
  pinMode(this->MOSI, OUTPUT);

  digitalWrite(this->CS, HIGH);
  digitalWrite(this->CLK, LOW);
  digitalWrite(this->MOSI, LOW);

  write_reg(DISPLAY_TEST_REG, 0);// set these addresses to certain value
  write_reg(SCAN_LIMIT_REG, 7);
  write_reg(DECODE_MODE_REG, 0);
  write_reg(SHUTDOWN_REG, 1);

  clear();
}

void LEDMatrixChip::clear()
{
  for (int i = 0; i < 8; i++) {
    write_reg(i + 1, 0x00);
  }
}

void LEDMatrixChip::turnOn(int xCol, int yRow, int delayTime)
{
  write_reg(row_index + 1, 0x01 << col_index);
}

void LEDMatrixChip::OnRow(int yRow, int delayTime)
{
  write_reg(row_index + 1, 0xFF);
}

void LEDMatrixChip::OnCol(int xCol, int delayTime)
{
  for (int i = 0; i < 8; i++) {
    write_reg(i + 1, 0x01 << col_index);
  }
}

// private :
void LEDMatrixChip::transfer(uint8_t *p_data, uint8_t len)
{
  uint8_t mask;

  digitalWrite(CS, LOW);//pull low to enable the chip to be selected
  delayMicroseconds(1);//delay

  for (int i = 0; i < len; i++)//iterate through the bytes
  {
    mask = 0x80;//1000, 0000 in binary, the bit checker
    do
    {
      if (p_data[i] & mask)//nth byte == checking byte
      {
        digitalWrite(MOSI, HIGH);//write high to chip
      }
      else// nth byte != checking byte
      {
        digitalWrite(MOSI, LOW);//write low to chip
      }

      delayMicroseconds(1);//delay
      digitalWrite(CLK, HIGH);//lock in the data
      delayMicroseconds(1);//delay
      digitalWrite(CLK, LOW);//release, wait for next data

      mask >>= 1;// shift checking bit by 1 to the right
    } while (mask != 0);// repeat until the bit checker = 0 
  }

  digitalWrite(CS, HIGH);// deselect the chip
}

void LEDMatrixChip::write_reg(uint8_t reg, uint8_t value)
{
  uint8_t tx_data[2] = { reg, value };//array of input valus(address, value)
  transfer(tx_data, 2);//send the data into the chip
}

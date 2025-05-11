#include "LEDMatrix.h"

// Constructor: Initializes pins and memory
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

// Show a custom column pattern with optional shift
template <size_t ColSize, size_t RowSize>
void LEDMatrix<ColSize, RowSize>::customCol(int array[], int Col, int shift, int delayTime)
{
    adjustShift(shift, array, 0);
    for (size_t i = 0; i < RowSize; i++)
        pinReq[i] = OutputArray[i];

    for (size_t i = 0; i < ColSize; i++)
        pinReq[i + RowSize] = (i == limitingGrid(Col, 0)) ? 0 : 1;

    setPins();
    delay(delayTime);
}

// Show a custom row pattern with optional shift
template <size_t ColSize, size_t RowSize>
void LEDMatrix<ColSize, RowSize>::customRow(int array[], int Row, int shift, int delayTime)
{
    adjustShift(shift, array, 1);
    for (size_t i = 0; i < RowSize; i++)
        pinReq[i] = (i == limitingGrid(Row, 1)) ? 1 : 0;

    for (size_t i = 0; i < ColSize; i++)
        pinReq[i + RowSize] = OutputArray[i] ? 0 : 1;

    setPins();
    delay(delayTime);
}

// Run a basic test by cycling through rows and columns
template <size_t ColSize, size_t RowSize>
void LEDMatrix<ColSize, RowSize>::Test(int delayTime)
{
    for (size_t i = 0; i < NumPins; i++)
        digitalWrite(Pins[i], LOW);

    for (size_t i = 0; i < ColSize; i++)
    {
        digitalWrite(Pins[i], HIGH);
        delay(delayTime);
    }

    for (size_t i = 0; i < NumPins; i++)
        digitalWrite(Pins[i], HIGH);

    for (size_t i = 0; i < RowSize; i++)
    {
        digitalWrite(Pins[ColSize + i], LOW);
        delay(delayTime);
    }
}

// Display a full matrix symbol for a set duration
template <size_t ColSize, size_t RowSize>
void LEDMatrix<ColSize, RowSize>::Symbol(int UserMatrix[RowSize][ColSize], unsigned long showTime, bool clear)
{
    int TransformedMatrix[ColSize][RowSize] = {0};

    for (size_t i = 0; i < RowSize; i++)
        for (size_t j = 0; j < ColSize; j++)
            TransformedMatrix[ColSize - 1 - j][i] = UserMatrix[i][ColSize - 1 - j];

    unsigned long start = millis();
    while (millis() - start < showTime)
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
            digitalWrite(Pins[i], LOW);
    }
}

// Limit a value to within valid row/column bounds
template <size_t ColSize, size_t RowSize>
size_t LEDMatrix<ColSize, RowSize>::limitingGrid(int value, bool axis)
{
    int max = (!axis) ? ColSize : RowSize;
    return (size_t)constrain(value, 0, max - 1);
}

// Shift the pattern for rows/columns
template <size_t ColSize, size_t RowSize>
void LEDMatrix<ColSize, RowSize>::adjustShift(int shift, int array[], bool axis)
{
    int len = (!axis) ? ColSize : RowSize;
    shift = constrain(shift, -len, len);

    if (shift > 0)
    {
        for (int i = 0; i < shift; i++)
            OutputArray[i] = 0;
        for (int i = 0; i < len - shift; i++)
            OutputArray[i + shift] = array[i];
    }
    else if (shift < 0)
    {
        for (int i = 0; i < len + shift; i++)
            OutputArray[i] = array[i - shift];
        for (int i = len + shift; i < len; i++)
            OutputArray[i] = 0;
    }
    else
    {
        memcpy(OutputArray, array, len * sizeof(int));
    }
}

// Apply pin states using pinReq, only if changed
template <size_t ColSize, size_t RowSize>
void LEDMatrix<ColSize, RowSize>::setPins()
{
    for (size_t i = 0; i < NumPins; i++)
    {
        if (pinPrev[i] != pinReq[i])
        {
            pinPrev[i] = pinReq[i];
            digitalWrite(Pins[i], pinReq[i] ? HIGH : LOW);
        }
    }
}

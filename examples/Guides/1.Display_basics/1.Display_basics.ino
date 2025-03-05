#include <LEDMatrix.h>

// Define row and column pins (Order matters, follow your datasheet)
int posPin[] = {2, 3, 4, 5, 6, 7, 8, 9};         // Positive (Column) pins
int negPin[] = {10, 11, 12, 13, A0, A1, A2, A3}; // Negative (Row) pins

// Initialization: LEDMatrix<ColSize, RowSize> ObjectName(posPinArray, negPinArray)
LEDMatrix<8, 8> LM(posPin, negPin);

/*
  Class name <num_of_columns, num_of_rows>, Object_name(
  positive_pin_array[],
  negative_pin_array[]
  )
*/

// Custom row/column patterns (each value represents an LED state: 1 = ON, 0 = OFF)
int Example_Col[] = {0, 0, 0, 1, 1, 1, 0, 0}; // Example column pattern
int Example_Row[] = {1, 1, 1, 0, 0, 0, 1, 1}; // Example row pattern

void setup()
{
  LM.Test();
  /*
    Runs a test sequence to verify wiring.
    If rows and columns light up in order, pin arrays are correctly listed.
  */
}

void loop()
{
  LM.turnOn(2, 4);
  /*
    Turns on a single LED at a specific position.
    Parameters:
    Column index (0-7),
    Row index (0-7)
  */

  LM.OnCol(3);
  /*
    Turns on all LEDs in a specific column.
    Parameters:
    Column index (0-7)
  */

  LM.OnRow(6);
  /*
    Turns on all LEDs in a specific row.
    Parameters:
    Row index (0-7)
  */

  LM.customCol(Example_Col, 1);
  /*
    Displays a custom column pattern.
    Parameters:
    Array of LED states (1 = ON, 0 = OFF),
    Column index (0-7),
    Shift (default 0)
  */

  LM.customRow(Example_Row, 2);
  /*
    Displays a custom row pattern.
    Parameters:
    Array of LED states (1 = ON, 0 = OFF),
    Row index (0-7),
    Shift (default 0)
  */

  // LM.Symbol(2D array); // Displays a full custom 8x8 symbol (not used in this example)
}

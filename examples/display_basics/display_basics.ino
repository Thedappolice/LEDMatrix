#include <LEDMatrix.h>

// (Row/Column) for this example pins can be put in preffered order, follow your datasheet.
int posPin[] = { 2, 3, 4, 5, 6, 7, 8, 9 };
int negPin[] = { 10, 11, 12, 13, A0, A1, A2, A3 };

// Custom Row/Column array Initialization
int Example_Col[] = {0, 0, 0, 1, 1, 1, 0, 0};
int Example_Row[] = {1, 1, 1, 0, 0, 0, 1, 1};

LEDMatrix LM(posPin, 8, negPin, 8); // Initialization
/*
  Class name, Object_name(
  positive_pin_array[],
  size of positive_pin_array[],
  negative_pin_array[],
  size of positive_pin_array[]
  )
*/

void setup()
{
  LM.Test(); // test the LED Matrix
             // if rows and columns light up in order then pin array is listed in order
}

void loop()
{
  LM.turnOn(2, 4); // turn on x-th Column's, y-th Row's Led of Matrix

  LM.OnCol(3); // turn on entire n-th Column
  LM.OnRow(6); // turn on entire n-th Row

  LM.customCol(Example_Col, 1); // turn on 1st column with given array order
  /*
    Object.customCol(
    array_name,
    n-th_column,
    shift_left/right(optional)
  )
  Eg : LM.customCol(Example_Col, 1, 1)
                              //3rd argument is optional, can be positive/negative
                              //shifts the array by +/- n to left/right
  */

  LM.customRow(Example_Row, 2); // turn on 2nd row with given array order
  /*
    Object.customRow(
    array_name,
    n-th_row,
    shift_left/right(optional)
  )
  Eg : LM.customRow(Example_Row, 4, 6)
                              //3rd argument is optional, can be positive/negative
                              //shifts the array by +/- n to left/right
  */

  // LM.Symbol(2D array), read the Custom_char example
}
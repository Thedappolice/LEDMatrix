#include <LEDMatrix.h>

// (Row/Column) for this example pins can be put in preffered order, follow your datasheet.
int posPin[] = {2, 3, 4, 5, 6, 7, 8, 9};
int negPin[] = {10, 11, 12, 13, A0, A1, A2, A3};

// Custom Row/Column array Initialization
int Example_Col[] = {0, 0, 0, 1, 1, 1, 0, 0};
int Example_Row[] = {1, 1, 1, 0, 0, 0, 1, 1};

LEDMatrix LM<8, 8>(posPin, negPin); // Initialization
/*
  Class name <size_of_pos_pins, size_of_neg_pins>, Object_name(
  positive_pin_array[],
  negative_pin_array[]
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
  // LM.turnOn(2, 4, 1000); // turn on x-th Column's, y-th Row's Led of Matrix, for n milliseconds

  LM.OnCol(3); // turn on entire n-th Column
  // LM.OnCol(3, 1000); // turn on entire n-th Column, for n milliseconds

  LM.OnRow(6); // turn on entire n-th Row
  // LM.OnRow(6); // turn on entire n-th Row, for n milliseconds

  LM.customCol(Example_Col, 1); // turn on 1st column with given array order
  /*
    Object.customCol(
    array_name,
    n-th_column,
    shift_left/right(optional)
    display_duration(optional, defaulted to 1000, in milliseconds unit)
  )*/

  LM.customRow(Example_Row, 2); // turn on 2nd row with given array order
                                /*
                                   Object.customRow(
                                   array_name,
                                   n-th_column,
                                   shift_left/right(optional)
                                   display_duration(optional, defaulted to 1000, in milliseconds unit)
                                 )*/

  // LM.Symbol(2D array), read the Custom_char example
}
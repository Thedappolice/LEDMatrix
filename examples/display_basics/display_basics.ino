#include <LEDMatrix.h>

// (Row/Column) for this example pins can be put in preffered order, follow your datasheet.
int posPin[] = {6, 7, 8, 9, 10, 11, 12, 13};
int negPin[] = {A0, A1, A2, A3, A4, A5, A6, A7};

LEDMatrix LM(posPin, 8, negPin, 8); // Initialization
/*Class name, Object_name(
  positive_pin_array[],
  size of positive_pin_array[],
  negative_pin_array[],
  size of positive_pin_array[])
*/

void setup()
{
    LM.Test(); // test the LED Matrix
    // if rows and columns light up in order then pin array is listed in order
}

void loop()
{
    LM.turnOn(2, 4); // turn on x-th Column's, y-th Row's Led of Matrix
    LM.OnCol(3);     // turn on entire n-th Column
    LM.OnRow(6);     // turn on entire n-th Row
    //LM.Symbol(2D array), read the Custom_char example
}
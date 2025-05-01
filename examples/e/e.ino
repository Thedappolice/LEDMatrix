#include "template.h"

Printer<int> intPrinter;      // Printer for integers
Printer<float> floatPrinter;  // Printer for floats

void setup() {
  Serial.begin(9600);

  // Print an integer
  intPrinter.printNumber(123);

  // Print a floating-point number
  floatPrinter.printNumber(45.67);
}

void loop() {
  // No code in the loop for this simple example
}
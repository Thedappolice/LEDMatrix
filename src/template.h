// template.h
#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <Arduino.h>

template <typename T>
class Printer {
  public:
    // Constructor
    Printer() {}

    // Function to print the number to the serial monitor
    void printNumber(T number) {
        Serial.print("The number is: ");
        Serial.println(number);
    }
};

#endif

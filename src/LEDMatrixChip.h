#ifndef LEDMATRIXCHIP_H
#define LEDMATRIXCHIP_H

#include <Arduino.h>

class LEDMatrixChip 
{
public:
    // Constructor
    LEDMatrixChip(int CS, int CLK, int MOSI);

    //clean up display
    void clear();

    // // Turn on x-Col's, y-Row's LED
    // void turnOn(int Col, int Row);

    // // Turn on entire x-Col
    // void OnCol(int Col);

    // // Turn on entire y-Row
    // void OnRow(int Row);

    // // Turn on x-Col by given array, shiftable by +/- n
    // void customCol(int array[], int Col, int shift = 0);

    // // Turn on y-Row by given array, shiftable by +/- n
    // void customRow(int array[], int Row, int shift = 0);

    // Check all possible LED
    void Test();

    // // Display custom symbol
    // void Symbol(int UserMatrix[8][8]);

private:
    // Pin variables for chip communication
    int CS;
    int CLK;
    int MOSI;

    // Private methods for handling chip communication
    void transfer(uint8_t *p_data, uint8_t len); // Private transfer function to send data to the chip
    void write_reg(uint8_t reg, uint8_t value);  // Send a value to a specific register on the chip
};

#endif // LEDMATRIXCHIP_H

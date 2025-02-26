#ifndef LEDMATRIXCHIP_H
#define LEDMATRIXCHIP_H

#include <Arduino.h>

class LEDMatrixChip 
{
public:
    // Constructor
    LEDMatrixChip(int CS, int CLK, int MOSI, int amount = 1);

    //clean up display
    void clear();

    // Turn on x-Col's, y-Row's LED
    void turnOn(int Col, int Row);

    // // Turn on entire x-Col
    void OnCol(int Col);

    // // Turn on entire y-Row
    void OnRow(int Row);

    // // Turn on x-Col by given array, shiftable by +/- n
    void customCol(byte userByte, int Col, int shift = 0);

    // // Turn on y-Row by given array, shiftable by +/- n
    void customRow(byte userByte, int Row, int shift = 0);

    // Check all possible LED
    void Test();

    // // Display custom symbol
    void Symbol(byte UserMatrix[8]);

private:
    // Pin variables for chip communication
    int CS;
    int CLK;
    int MOSI;
    int amount;

    byte displayByte = 0x00;

    // Private methods for handling chip communication
    void transfer(uint8_t *p_data, uint8_t len); // Private transfer function to send data to the chip
    void write_reg(uint8_t reg, uint8_t value);  // Send a value to a specific register on the chip
    size_t limitingGrid(int value);    // Limit the grid to 0-7
    void adjustShift(int shift, byte userArray); // Shift the array by n
};

#endif // LEDMATRIXCHIP_H

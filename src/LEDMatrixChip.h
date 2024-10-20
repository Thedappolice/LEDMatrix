#ifndef LEDMATRIXCHIP_H
#define LEDMATRIXCHIP_H

#include <Arduino.h>
#include "LEDMatrix.h"  // Assuming you have the LEDMatrix base class

class LEDMatrixChip : public LEDMatrix<8, 8> // Inherit from LEDMatrix<8, 8>
{
public:
    // Constructor
    LEDMatrixChip(int CS, int CLK, int MOSI);

    // Methods
    void clear();  // Removed 'override' to avoid error
    void turnOn(int xCol, int yRow, int delayTime = 2);
    void OnRow(int yRow, int delayTime = 2);
    void OnCol(int xCol, int delayTime = 2);

private:
    // Pin variables for chip communication
    int CS;
    int CLK;
    int MOSI;

    // Private methods for handling chip communication
    void transfer(uint8_t *p_data, uint8_t len);
    void write_reg(uint8_t reg, uint8_t value);
};

#endif // LEDMATRIXCHIP_H

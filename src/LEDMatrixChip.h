// #ifndef LEDMatrix_H
// #define LEDMatrix_H

// #include <Arduino.h>

// class LEDMatrixChip : public LEDMatrix<8, 8> // use ': public' for inheritance
// {
// public:
//     LEDMatrixChip(int CS, int CLK, int MOSI); // constructor
//     void clear() override;
//     void turnOn(int xCol, int yRow, int delayTime = 2) override; // turn on x-Col's, y-Row's LED
//     void OnRow(int yRow, int delayTime = 2) override;            // turn on entire y-Row
//     void OnCol(int xCol, int delayTime = 2) override;            // turn on entire x-Col

// private:
//     int CS;
//     int CLK;
//     int MOSI;
//     void transfer(uint8_t *p_data, uint8_t len);
//     void write_reg(uint8_t reg, uint8_t value);
// };

// #endif

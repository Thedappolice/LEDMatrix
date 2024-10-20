#include "LEDMatrixChip.h"

// Register constants for the LED matrix driver
#define DECODE_MODE_REG 0x09
#define INTENSITY_REG 0x0A
#define SCAN_LIMIT_REG 0x0B
#define SHUTDOWN_REG 0x0C
#define DISPLAY_TEST_REG 0x0F

// Constructor with initialization of the base class LEDMatrix
LEDMatrixChip::LEDMatrixChip(int CS, int CLK, int MOSI)
    : LEDMatrix<8, 8>(nullptr, nullptr)  // Pass nullptr or arrays for posPins and negPins
{
    this->CS = CS;
    this->CLK = CLK;
    this->MOSI = MOSI;

    pinMode(this->CS, OUTPUT);
    pinMode(this->CLK, OUTPUT);
    pinMode(this->MOSI, OUTPUT);

    digitalWrite(this->CS, HIGH);
    digitalWrite(this->CLK, LOW);
    digitalWrite(this->MOSI, LOW);

    write_reg(DISPLAY_TEST_REG, 0);
    write_reg(SCAN_LIMIT_REG, 7);
    write_reg(DECODE_MODE_REG, 0);
    write_reg(SHUTDOWN_REG, 1);

    clear(); // Clear the display on initialization
}


void LEDMatrixChip::clear()
{
    for (int i = 0; i < 8; i++)
    {
        write_reg(i + 1, 0x00); // Clear each row by setting it to 0
    }
}

void LEDMatrixChip::turnOn(int xCol, int yRow, int delayTime)
{
    // Ensure the column and row are within bounds
    if (xCol >= 0 && xCol < 8 && yRow >= 0 && yRow < 8)
    {
        // Write to the row register, turning on the specific column bit
        write_reg(yRow + 1, 0x01 << xCol);
        delay(delayTime); // Optional delay
    }
}

void LEDMatrixChip::OnRow(int yRow, int delayTime)
{
    // Ensure the row is within bounds
    if (yRow >= 0 && yRow < 8)
    {
        // Turn on all columns in the given row
        write_reg(yRow + 1, 0xFF);
        delay(delayTime); // Optional delay
    }
}

void LEDMatrixChip::OnCol(int xCol, int delayTime)
{
    // Ensure the column is within bounds
    if (xCol >= 0 && xCol < 8)
    {
        // Turn on the same column for all rows
        for (int i = 0; i < 8; i++)
        {
            write_reg(i + 1, 0x01 << xCol);
        }
        delay(delayTime); // Optional delay
    }
}

// Private transfer function to send data to the chip
void LEDMatrixChip::transfer(uint8_t *p_data, uint8_t len)
{
    uint8_t mask;

    digitalWrite(CS, LOW); // Select the chip
    delayMicroseconds(1);  // Small delay

    for (int i = 0; i < len; i++) // Iterate through the bytes
    {
        mask = 0x80; // Start with the most significant bit
        do
        {
            // Set MOSI high or low based on the current bit
            digitalWrite(MOSI, (p_data[i] & mask) ? HIGH : LOW);

            delayMicroseconds(1);
            digitalWrite(CLK, HIGH); // Clock in the data
            delayMicroseconds(1);
            digitalWrite(CLK, LOW);  // Prepare for next bit

            mask >>= 1; // Shift the mask to the next bit
        } while (mask != 0); // Continue until all bits are sent
    }

    digitalWrite(CS, HIGH); // Deselect the chip
}

// Send a value to a specific register on the chip
void LEDMatrixChip::write_reg(uint8_t reg, uint8_t value)
{
    uint8_t tx_data[2] = {reg, value}; // Prepare the register and value
    transfer(tx_data, 2);              // Transfer the data to the chip
}

#include "LEDMatrixChip.h"

// Register constants for the LED matrix driver
#define DECODE_MODE_REG 0x09
#define INTENSITY_REG 0x0A
#define SCAN_LIMIT_REG 0x0B
#define SHUTDOWN_REG 0x0C
#define DISPLAY_TEST_REG 0x0F

// Constructor with initialization of the base class LEDMatrix
LEDMatrixChip::LEDMatrixChip(int CS, int CLK, int MOSI, int amount)
{
    this->CS = CS;
    this->CLK = CLK;
    this->MOSI = MOSI;
    this->amount = amount;

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
    write_reg(DISPLAY_TEST_REG, 0);

    clear(); // Clear the display on initialization
}

void LEDMatrixChip::clear()
{
    for (int i = 0; i < 8; i++)
    {
        write_reg(i + 1, 0x00); // Clear each row by setting it to 0
    }
}

void LEDMatrixChip::turnOn(int Col, int Row)
{
    // Write to the row register, turning on the specific column bit
    write_reg(limitingGrid(Row) + 1, 0x01 << limitingGrid(Col));
}

void LEDMatrixChip::OnCol(int Col)
{
    // Turn on the same column for all rows
    for (int i = 0; i < 8; i++)
    {
        write_reg(i + 1, 0x01 << limitingGrid(Col));
    }
}

void LEDMatrixChip::OnRow(int Row)
{
    // Turn on all columns in the given row
    write_reg(limitingGrid(Row) + 1, 0xFF);
}

void LEDMatrixChip::customCol(byte userByte, int Col, int shift)
{
    adjustShift(shift, userByte);

    for (int i = 0; i < 8; i++)
    {
        // Turn on the column with the shifted byte
        write_reg(i + 1, (userByte & (0x01 << i)) ? (0x01 << limitingGrid(Col)) : 0);
    }
}

void LEDMatrixChip::customRow(byte userByte, int Row, int shift = 0)
{
    adjustShift(shift, userByte);
    write_reg(Row + 1, displayByte);
}

void LEDMatrixChip::Test()
{
    write_reg(DISPLAY_TEST_REG, 1); // Enable the test mode
    delay(500);                     // Wait for a second
    write_reg(DISPLAY_TEST_REG, 0); // Disable the test mode
    delay(500);                     // Wait for a second
}

void LEDMatrixChip::Symbol(byte UserMatrix[8])
{
    for (int i = 0; i < 8; i++)
    {
        write_reg(i + 1, UserMatrix[i]);
    }
}

// private :
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
            digitalWrite(CLK, LOW); // Prepare for next bit

            mask >>= 1; // Shift the mask to the next bit
        } while (mask != 0); // Continue until all bits are sent
    }

    digitalWrite(CS, HIGH); // Deselect the chip
}

void LEDMatrixChip::write_reg(uint8_t reg, uint8_t value)
{
    uint8_t tx_data[2] = {reg, value}; // Prepare the register and value
    transfer(tx_data, 2);              // Transfer the data to the chip
}

size_t LEDMatrixChip::limitingGrid(int value)
{
    // Constrain the value to be within 0 and the maximum index
    return constrain(value, 0, 7);
}

void LEDMatrixChip::adjustShift(int shift, byte userArray)
{
    // Constrain the shift value to prevent out-of-bound shifts
    shift = constrain(shift, -7, 7);
    displayByte = userArray;

    if (shift > 0) // Shift upwards/leftwards
    {
        displayByte = displayByte << shift;
    }
    if (shift < 0) // Shift downwards/rightwards
    {
        displayByte = displayByte >> shift;
    }
}
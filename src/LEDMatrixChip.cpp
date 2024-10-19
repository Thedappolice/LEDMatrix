// #include "LEDMatrix.h"

// // chipped version
// // public :

// #define DECODE_MODE_REG 0x09
// #define INTENSITY_REG 0x0A
// #define SCAN_LIMIT_REG 0x0B
// #define SHUTDOWN_REG 0x0C
// #define DISPLAY_TEST_REG 0x0F

// LEDMatrixChip::LEDMatrixChip(int CS, int CLK, int MOSI)
// {
//   this->CS = CS;
//   this->CLK = CLK;
//   this->MOSI = MOSI;

//   pinMode(this->CS, OUTPUT);
//   pinMode(this->CLK, OUTPUT);
//   pinMode(this->MOSI, OUTPUT);

//   digitalWrite(this->CS, HIGH);
//   digitalWrite(this->CLK, LOW);
//   digitalWrite(this->MOSI, LOW);

//   write_reg(DISPLAY_TEST_REG, 0);
//   write_reg(SCAN_LIMIT_REG, 7);
//   write_reg(DECODE_MODE_REG, 0);
//   write_reg(SHUTDOWN_REG, 1);

//   clear();
// }

// void LEDMatrixChip::clear()
// {
//   for (int i = 0; i < 8; i++)
//   {
//     write_reg(i + 1, 0x00);
//   }
// }

// void LEDMatrixChip::turnOn(int xCol, int yRow, int delayTime)
// {
//   write_reg(row_index + 1, 0x01 << col_index);
// }

// void LEDMatrixChip::OnRow(int yRow, int delayTime)
// {
//   write_reg(row_index + 1, 0xFF);
// }

// void LEDMatrixChip::OnCol(int xCol, int delayTime)
// {
//   for (int i = 0; i < 8; i++)
//   {
//     write_reg(i + 1, 0x01 << col_index);
//   }
// }

// // private :
// void LEDMatrixChip::transfer(uint8_t *p_data, uint8_t len)
// {
//   uint8_t mask; // checking byte

//   digitalWrite(CS, LOW); // pull low to enable the chip to be selected
//   delayMicroseconds(1);  // delay

//   for (int i = 0; i < len; i++) // iterate through the bytes
//   {
//     mask = 0x80; // 1000, 0000 in binary, the bit checker
//     do
//     {
//       if (p_data[i] & mask) // nth byte == checking byte
//       {
//         digitalWrite(MOSI, HIGH); // write high to chip
//       }
//       else // nth byte != checking byte
//       {
//         digitalWrite(MOSI, LOW); // write low to chip
//       }

//       delayMicroseconds(1);    // delay
//       digitalWrite(CLK, HIGH); // lock in the data
//       delayMicroseconds(1);    // delay
//       digitalWrite(CLK, LOW);  // release, wait for next data

//       mask >>= 1; // shift checking bit by 1 to the right
//     } while (mask != 0); // repeat until the bit checker = 0
//   }

//   digitalWrite(CS, HIGH); // deselect the chip
// }

// void LEDMatrixChip::write_reg(uint8_t reg, uint8_t value)
// {
//   uint8_t tx_data[2] = {reg, value}; // array of input valus(address, value)
//   transfer(tx_data, 2);              // send the data into the chip
// }

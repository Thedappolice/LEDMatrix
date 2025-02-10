// file.h
#ifndef RIPTstris_NO_CHIP_H // Include guard
#define RIPTetris_NO_CHIP_H

#include <Arduino.h>

// Function prototype

class Tetris_No_Chip
{
public:
    Tetris_No_Chip();

    ~Tetris_No_Chip();

    void Char(LEDMatrix<8, 8> &LM, char input, unsigned long duration = 0);

    void alterShape(int mode);

    void genShape();

    void clearRow(int row);

    void shiftRowsDown(int startRow);

    void scanAndClearGrid();

    void checkInput();

    void gatherAndDisplay();

    void showEndAnimation();

    void sendScore(int score);

    void setup();

    void loop();

private:
    int displayMatrix[][8];
    LEDMatrix<8, 8> *topLM;
    LEDMatrix<8, 8> *botLM;
}

#endif

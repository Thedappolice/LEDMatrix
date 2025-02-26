#include <LEDMatrix.h>
#include <LEDMatrixChip.h>

#define DATA_PIN 2
#define CHIP_SELECT_PIN 3
#define CLOCK_PIN 4

LEDMatrixChip LM(CHIP_SELECT_PIN, CLOCK_PIN, DATA_PIN);

byte symbol[] = {
    0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e};

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
}
void loop()
{
    LM.Symbol(symbol);
}
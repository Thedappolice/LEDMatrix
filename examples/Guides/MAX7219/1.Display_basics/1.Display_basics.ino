#include <LEDMatrix.h>
#include <LEDMatrixChip.h>

#define DATA_PIN 2
#define CHIP_SELECT_PIN 3
#define CLOCK_PIN 4

LEDMatrixChip LM(CHIP_SELECT_PIN, CLOCK_PIN, DATA_PIN);

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
}
void loop()
{
    LM.Test();
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    LM.clear();
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
}
I'msee#include <LEDMatrix.h>

int posPin[] = {2, 3, 4, 5, 6, 7, 8, 9};
int negPin[] = {10, 11, 12, 13, A0, A1, A2, A3};

LEDMatrix LM(posPin, 8, negPin, 8);

int headX;
int headY;

int End[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 0, 0, 1, 0, 0, 1},
    {1, 0, 0, 1, 0, 1, 1, 0},
    {1, 1, 1, 1, 0, 1, 0, 1},
    {0, 0, 0, 0, 0, 0, 0, 0},
};

int N1[8][8] = {
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 0, 0},
    {0, 1, 1, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 0}};
int N2[8][8] = {
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 0, 0, 1, 1, 0},
    {0, 0, 0, 0, 1, 1, 1, 0},
    {0, 0, 0, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 0}};
int N3[8][8] = {
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 0, 0, 0, 1, 1, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 0, 0, 0, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 1, 1, 1, 1, 0, 0}};

int displayM[8][8] = {{0}};
int memory[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 2, 0, 0, 0},
    {0, 0, 0, 0, 3, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
};

bool end = false;
string direction;


void MemtoDisplay()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (memory[i][j] != 0)
            {
                displayM[i][j] = 1;
            }
        }
    }
};

void checkJST()
{
if(analogRead(A5) > 512)
{
direction = 'right';
}else
{
if(analogRead(A5) < 512)
{
direction = 'left';
}else
if(analogRead(A6) > 512)
{
direction = 'up';
}else
if(analogRead(A6) < 512)
{
direction = 'down';
}
};

void updateMem()
{
for(int i =0; i <8;i++)
{
for(int j =0; j<8;j++)
{
if(memory[i][j])
}
}
};

void setup()
{
digitalWrite(A4,HIGH);
    MemtoDisplay();
    displaywithtime(N3); // countdown
    displaywithtime(N2);
    displaywithtime(N1);
    for (int i = 0; i < 20; i++) // blink the ball position, showing the starting position
    {
        LM.Symbol(displayM);
        delay(100);
    }
}
void loop()
{
    while (!end)
    {

    }
}
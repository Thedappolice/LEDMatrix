#include <LEDMatrix.h>

int posPin[] = {6, 7, 8, 9, 10, 11, 12, 13};
int negPin[] = {A0, A1, A2, A3, A4, A5, A6, A7};

int wall[] = {0, 0, 0, 1, 1, 1, 0, 0};

LEDMatrix LM(posPin, 8, negPin, 8);

#define P1left 4
#define P1right 5
#define P2left 2
#define P2right 3

int ballX;
int ballY;
int Xdirection = random(0, 1);
int Ydirection = random(0, 1);

int P1leftstat;
int P1rightstat;
int P2leftstat;
int P2rightstat;

int P1shift = 0;
int P2shift = 0;

int memory[8][8] =
    {
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0}};

void setup()
{
    for (int i = 2; i < 6; i++)
    {
        pinMode(i, INPUT);
    }
}

void loop()
{
    display();
    checkbutton();
    checklogic(memory);
}

void checkbutton()
{
    P1leftstat = digitalRead(P1left);
    P1rightstat = digitalRead(P1right);
    P2leftstat = digitalRead(P2left);
    P2rightstat = digitalRead(P2right);
};

void display()
{
    LM.customCol(wall, 0, P1shift);
    LM.customCol(wall, 8, P2shift);
    LM.turnOn(ballX, ballY);
};

void checklogic(int memory[][8])
{
    if (ballX == 1 || ballX == 6)
    {
    }
};

int limitingshift(int value, bool change)
{
    int mem;
    if (value < 3 || change == true)
    {
        mem = value++;
    }
    else if (value > -4 || change == false)
    {
        mem = value--;
    }
    return mem;
}

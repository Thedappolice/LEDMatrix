#include <LEDMatrix.h>

int posPin[] = {2, 3, 4, 5, 6, 7, 8, 9};
int negPin[] = {10, 11, 12, 13, A0, A1, A2, A3};

LEDMatrix LM(posPin, 8, negPin, 8);

int JoyX;
int JoyY;

unsigned long timeupdate;

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
String direction = "up";

void displaywithtime(int Matrix[][8], int time = 1000)
{
  timeupdate = millis();
  while (millis() - timeupdate < time)
  {
    LM.Symbol(Matrix);
  }
};

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
    if (analogRead(A7) > 768)
    {
        direction = "left";
    }
    else if (analogRead(A7) < 256)
    {
        direction = "right";
    }
    else if (analogRead(A6) > 768)
    {
        direction = "down";
    }
    else if (analogRead(A6) < 256)
    {
        direction = "up";
    }
};

void updateMem()
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

void setup()
{
    Serial.begin(9600);
    MemtoDisplay();
    displaywithtime(N3); // countdown
    displaywithtime(N2);
    displaywithtime(N1);
    for (int i = 0; i < 20; i++) // blink the ball position, showing the starting position
    {
        LM.Symbol(displayM);
        delay(100);
    }
};

void loop()
{
    
    Serial.println(direction);
    // while (!end)
    // {
    //   checkJST();
    // }
}
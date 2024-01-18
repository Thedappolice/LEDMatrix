#include <LEDMatrix.h>

int posPin[] = {6, 7, 8, 9, 10, 11, 12, 13};
int negPin[] = {A0, A1, A2, A3, A4, A5, A6, A7};

int wall[] = {0, 0, 1, 1, 1, 0, 0, 0};

LEDMatrix LM(posPin, 8, negPin, 8);

#define buzzer 1

#define P1left 4
#define P1right 5
#define P2left 2
#define P2right 3

int timeupdate;

int ballX = 3;
int ballY = 3;
int Xdirection = 1;
int Ydirection = 1;

int P1leftstat;
int P1rightstat;
int P2leftstat;
int P2rightstat;

int P1change = 0;
int P2change = 0;

int P1shift = 0;
int P2shift = 0;

int memory[8][8];

int W[8][8] = {
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 1, 1, 0, 0, 1},
    {1, 1, 0, 1, 1, 0, 1, 1},
    {1, 1, 0, 1, 1, 0, 1, 1},
    {1, 1, 1, 0, 0, 1, 1, 1},
    {0, 1, 1, 0, 0, 1, 1, 0},
    {0, 1, 0, 0, 0, 0, 1, 0}};

int I[8][8] = {
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 0}};

int N[8][8] = {
    {1, 1, 0, 0, 0, 0, 1, 1},
    {1, 1, 1, 0, 0, 0, 1, 1},
    {1, 1, 1, 1, 0, 0, 1, 1},
    {1, 1, 1, 1, 1, 0, 1, 1},
    {1, 1, 0, 1, 1, 1, 1, 1},
    {1, 1, 0, 0, 1, 1, 1, 1},
    {1, 1, 0, 0, 0, 1, 1, 1},
    {1, 1, 0, 0, 0, 0, 1, 1}};

int P[8][8] = {
    {0, 1, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 0, 0, 0, 1, 0},
    {0, 1, 1, 0, 0, 0, 1, 0},
    {0, 1, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 0, 0, 0, 0, 0},
    {0, 1, 1, 0, 0, 0, 0, 0},
    {0, 1, 1, 0, 0, 0, 0, 0},
    {0, 1, 1, 0, 0, 0, 0, 0}};

int P1[8][8] = {
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 0, 0},
    {0, 1, 1, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 0}};

int P2[8][8] = {
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 0, 0, 1, 1, 0},
    {0, 0, 0, 0, 1, 1, 1, 0},
    {0, 0, 0, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 0}};

void displaywithtime(int Matrix[][8], int time = 1000)
{
  timeupdate = millis();
  while (millis() - timeupdate < time)
  {
    LM.Symbol(Matrix);
  }
};

void checkbutton()
{
  P1leftstat = digitalRead(P1left);
  P1rightstat = digitalRead(P1right);
  P2leftstat = digitalRead(P2left);
  P2rightstat = digitalRead(P2right);
  if (P1change == 0)
  {
    if (P1leftstat == 1 || P1rightstat == 1)
    {
      if (P1leftstat == 1)
      {
        P1shift = limitingshift(P1shift, true);
      }
      else if (P1rightstat == 1)
      {
        P1shift = limitingshift(P1shift, false);
      }
      P1change = 1;
      Serial.println(P1shift);
    }
  }
  else
  {
    if (P1leftstat == 0 && P1rightstat == 0)
    {
      P1change = 0;
    }
  }
  if (P2change == 0)
  {
    if (P2leftstat == 1 || P2rightstat == 1)
    {
      if (P2leftstat == 1)
      {
        P2shift = limitingshift(P2shift, false);
      }
      else if (P2rightstat == 1)
      {
        P2shift = limitingshift(P2shift, true);
      }
      P2change = 1;
      Serial.println(P2shift);
    }
  }
  else
  {
    if (P2leftstat == 0 && P2rightstat == 0)
    {
      P2change = 0;
    }
  }
};

void display()
{
  LM.customRow(wall, 7, P1shift);
  LM.customRow(wall, 0, P2shift);
  LM.turnOn(ballX, ballY);
};

void checklogic(int memory[][8]){

};

int limitingshift(int value, bool change)
{
  int mem;
  if (value < 3 && change == true)
  {
    mem = value + 1;
  }
  else if (value > -2 && change == false)
  {
    mem = value - 1;
  }
  return mem;
};

void updateMem()
{
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      memory[j][i] = 0;
    }
  }
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      if (2 + P1shift == j || 3 + P1shift == j || 4 + P1shift == j)
      {
        memory[i][0] = 1;
      }
    }
  }
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      if (2 + P2shift == j || 3 + P2shift == j || 4 + P2shift == j)
      {
        memory[i][7] = 1;
      }
    }
  }
  memory[ballY][ballX] = 1;
};

void End(int result, bool simple = false)
{
  if (!simple)
  {
    for (int i = 0; i < 3; i++)
    {
      displaywithtime(P);
      if (result == 1)
      {
        displaywithtime(P1);
      }
      else if (result == 2)
      {
        displaywithtime(P2);
      }
      displaywithtime(W);
      displaywithtime(I);
      displaywithtime(N);
    }
  }
  else
  {
    for (int i = 0; i < 30; i++)
    {
      if (result == 1)
      {
        displaywithtime(P1, 300);
      }
      else if (result == 2)
      {
        displaywithtime(P2, 300);
      }
      delay(300);
    }
  }
  exit(1);
};

void setup()
{
  Serial.begin(9600);
  for (int i = 2; i < 6; i++)
  {
    pinMode(i, INPUT);
  }
  pinMode(1, OUTPUT);
};

void loop()
{
  updateMem();
  display();
  checkbutton();
};

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

bool balldelay = false;
unsigned long timeupdate;
int balldelaytime = 500;

int ballX = random(3, 5);
int ballY = random(3, 5);
int ballXdirection = 1;
int ballYdirection = 1;
int check;

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
  LM.turnOn(ballY, ballX);
};

void updateMem(bool normal = true)
{
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      memory[j][i] = 0;
    }
  }
  if (normal)
  {
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
  }
  memory[ballY][ballX] = 1;
};

void End(int result, bool simple = false)
{
  if (ballXdirection == 1)
  {
    ballX++;
  }
  else
  {
    ballX--;
  }
  updateMem(false);
  for (int i = 0; i < 6; i++)
  {
    displaywithtime(memory, 500);
  }
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

void ballchange()
{
  if (balldelay)
  {
    if (millis() - timeupdate > balldelaytime)
    {
      if (ballX == 1 || ballX == 6)
      {
        if (ballX == 1)
        {
          check = -1;
        }
        if (memory[ballY][ballX + check] != 1)
        {
          if (check == -1)
          {
            End(1, true);
          }
          else if (check == 1)
          {
            End(2, true);
          }
        }
        else
        {
          if (ballXdirection == 1)
          {
            ballXdirection = -1;
          }
          else if (ballXdirection == -1)
          {
            ballXdirection = 1;
          }
        }
      }
      if (ballXdirection == 1)
      {
        ballX++;
      }
      else
      {
        ballX--;
      }

      if (ballY == 0 || ballY == 7)
      {
        if (ballYdirection == 1)
        {
          ballYdirection = -1;
        }
        else if (ballYdirection == -1)
        {
          ballYdirection = 1;
        }
      }
      if (ballYdirection == 1)
      {
        ballY++;
      }
      else
      {
        ballY--;
      }

      balldelay = false;
    }
  }
  else
  {
    timeupdate = millis();
    balldelay = true;
  }
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

void setup()
{
  Serial.begin(9600);
  for (int i = 2; i < 6; i++)
  {
    pinMode(i, INPUT);
  }
  Serial.begin(9600);
};

void loop()
{
  updateMem();
  display();
  checkbutton();
  ballchange();
  // Serial.print(millis());
  // Serial.print(" - ");
  // Serial.print(timeupdate);
  // Serial.print(" = ");
  // Serial.println(millis() - timeupdate);
  // delay(1000);
};

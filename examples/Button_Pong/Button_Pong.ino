#include <LEDMatrix.h>

// Pin configurations for LED Matrix
int posPin[] = {2, 3, 4, 5, 6, 7, 8, 9};
int negPin[] = {10, 11, 12, 13, A0, A1, A2, A3};

// Wall pattern array
int wall[] = {0, 0, 1, 1, 1, 0, 0, 0};

// LED Matrix object initialization
LEDMatrix LM(posPin, 8, negPin, 8);

// Definitions for pin-like variables
#define P1left A4
#define P1right A5
#define P2left A7
#define P2right A6

// Time control variables
bool balldelay = false;
unsigned long timeupdate;
unsigned long balldelaytime = 1000;

// Ball control variables
int ballX = random(3, 5);
int ballY = random(3, 5);
int ballXdirection = 1;
int ballYdirection = 1;
int check;
int mem;

// Input detection variables
int P1leftstat;
int P1rightstat;
int P2leftstat;
int P2rightstat;
int P1change = 0;
int P2change = 0;

// Variables for array position alteration
int P1shift = 0;
int P2shift = 0;

// Game assets allocation
int memory[8][8] = {{0}};
bool simplicity = false; // Set to true for simpler ending display

// Game patterns for ending display
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

// Function to convert analog input to digital input
int checkstate(int value)
{
  if (value > 1000)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

// Function to limit player input
int limitingshift(int value, bool change)
{
  if (value < 3 && change == true)
  {
    mem = value + 1;
  }
  else if (value > -2 && change == false)
  {
    mem = value - 1;
  }
  return mem;
}

// Function to increase ball speed
void fasterball()
{
  balldelaytime = ceil(500 - millis() / 150);
}

// Function to check button inputs
void checkbutton()
{
  P1leftstat = checkstate(analogRead(P1left));
  P1rightstat = checkstate(analogRead(P1right));
  P2leftstat = checkstate(analogRead(P2left));
  P2rightstat = checkstate(analogRead(P2right));

  if (P1change == 0)
  {
    if (P1leftstat == 1 || P1rightstat == 1)
    {
      if (P1leftstat == 1)
      {
        P1shift = limitingshift(P1shift, false);
      }
      else if (P1rightstat == 1)
      {
        P1shift = limitingshift(P1shift, true);
      }
      P1change = 1;
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
        P2shift = limitingshift(P2shift, true);
      }
      else if (P2rightstat == 1)
      {
        P2shift = limitingshift(P2shift, false);
      }
      P2change = 1;
    }
  }
  else
  {
    if (P2leftstat == 0 && P2rightstat == 0)
    {
      P2change = 0;
    }
  }
}

// Function to display on the LED Matrix
void display()
{
  LM.customRow(wall, 0, P1shift);
  LM.customRow(wall, 7, P2shift);
  LM.turnOn(ballY, ballX);
}

// Function to update game memory
void updateMem(bool normal = true)
{
  // Clear memory
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      memory[j][i] = 0;
    }
  }

  if (normal)
  {
    // Update memory based on player shifts and ball position
    for (int j = 0; j < 8; j++)
    {
      if (2 + P1shift == j || 3 + P1shift == j || 4 + P1shift == j)
      {
        memory[j][0] = 1;
      }
    }

    for (int j = 0; j < 8; j++)
    {
      if (2 + P2shift == j || 3 + P2shift == j || 4 + P2shift == j)
      {
        memory[j][7] = 1;
      }
    }

    memory[ballY][ballX] = 1;
  }
}

// Function to end the game
void End(int result, bool simple = false)
{
  // Move the ball to final position
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

  updateMem(false); // Clear memory

  // Blink ball position for losing side indication
  for (int i = 0; i < 20; i++)
  {
    LM.turnOn(ballY, ballX);
    delay(100);
  }

  // Display ending symbols or numbers based on 'simple' flag
  if (!simple)
  {
    for (int i = 0; i < 3; i++)
    {
      LM.Symbol(P);
      if (result == 1)
      {
        LM.Symbol(N1);
      }
      else if (result == 2)
      {
        LM.Symbol(N2);
      }
      LM.Symbol(W);
      LM.Symbol(I);
      LM.Symbol(N);
    }
  }
  else
  {
    for (int i = 0; i < 30; i++)
    {
      if (result == 1)
      {
        LM.Symbol(N1, 300);
      }
      else if (result == 2)
      {
        LM.Symbol(N2, 300);
      }
      delay(300);
    }
  }

  exit(1); // Exit code; reset to restart
}

// Function to change ball position
void ballchange()
{
  if (balldelay)
  {
    if (millis() - timeupdate > balldelaytime)
    {
      // Check if ball reaches player edge
      if (ballX == 1 || ballX == 6)
      {
        if (ballX == 1)
        {
          check = -1;
        }
        else
        {
          check = 1;
        }

        // Check if player edge blocks the path
        if (memory[ballY][ballX + check] != 1)
        {
          if (check == -1)
          {
            End(2, simplicity);
          }
          else if (check == 1)
          {
            End(1, simplicity);
          }
        }
        else
        {
          // Change direction on player edge
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

      // Move ball
      if (ballXdirection == 1)
      {
        ballX++;
      }
      else
      {
        ballX--;
      }

      // Change direction if ball reaches top or bottom
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
    // Record previous time
    timeupdate = millis();
    balldelay = true;
  }
}

// Setup function
void setup()
{
  // Initial symbols on LED Matrix
  LM.Symbol(N3, 1000);
  LM.Symbol(N2, 1000);
  LM.Symbol(N1, 1000);

  // Blink ball position to indicate starting position
  for (int i = 0; i < 20; i++)
  {
    LM.turnOn(ballY, ballX);
    delay(100);
  }
}

// Loop function to call other functions sequentially
void loop()
{
  updateMem();
  display();
  checkbutton();
  ballchange();
  fasterball();
}

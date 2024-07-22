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
bool balldelayed = false;
unsigned long timeupdate;
unsigned long balldelaytime = 1000;

// Ball control variables
int ballX = random(3, 5);
int ballY = random(3, 5);
bool ballXdirPos = true;
bool ballYdirPos = true;

// Input detection variables
bool P1changed = false;
bool P2changed = false;

// Variables for array position alteration
int P1shift = 0;
int P2shift = 0;

// Game assets allocation
int memory[8][8] = {{0}};

void ShowSymbol(char input, unsigned long delay = 0)
{
  int display[8][8] = {{0}};
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

  switch (input)
  {
  case 'W':
    memcpy(display, W, 8 * 8 * sizeof(int));
    break;
  case 'I':
    memcpy(display, I, 8 * 8 * sizeof(int));
    break;
  case 'N':
    memcpy(display, N, 8 * 8 * sizeof(int));
    break;
  case 'P':
    memcpy(display, P, 8 * 8 * sizeof(int));
    break;
  case '1':
    memcpy(display, N1, 8 * 8 * sizeof(int));
    break;
  case '2':
    memcpy(display, N2, 8 * 8 * sizeof(int));
    break;
  case '3':
    memcpy(display, N3, 8 * 8 * sizeof(int));
    break;
  }

  if (delay != 0)
  {
    LM.Symbol(display, delay);
  }
  else
  {
    LM.Symbol(display);
  }
}

// Function to limit player input
int limitingshift(int value, bool positiveDir)
{
  int mem = 0;
  if (value < 3 && positiveDir)
  {
    mem = value + 1;
  }
  else if (value > -2 && !positiveDir)
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
  bool P1leftstat = analogRead(P1left) == HIGH;
  bool P1rightstat = analogRead(P1right) == HIGH;
  bool P2leftstat = analogRead(P2left) == HIGH;
  bool P2rightstat = analogRead(P2right) == HIGH;

  if (!P1changed)
  {
    if (P1leftstat)
    {
      P1shift = limitingshift(P1shift, false);
    }
    else if (P1rightstat)
    {
      P1shift = limitingshift(P1shift, true);
    }
    P1changed = true;
  }
  else
  {
    if (!P1leftstat && !P1rightstat)
    {
      P1changed = false;
    }
  }

  if (!P2changed)
  {
    if (P2leftstat)
    {
      P2shift = limitingshift(P2shift, true);
    }
    else if (P2rightstat)
    {
      P2shift = limitingshift(P2shift, false);
    }
    P2changed = true;
  }
  else
  {
    if (!P2leftstat && !P2rightstat)
    {
      P2changed = false;
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
void updateMem()
{
  // Clear memory
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      memory[j][i] = 0;
    }
  }

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

// Function to end the game
void End(bool isP1)
{
  // Move the ball to final position
  ballX = (ballXdirPos) ? ballX + 1 : ballX - 1;

  ballYdirPos = (ballY == 0 || ballY == 7) ? !ballYdirPos : ballYdirPos;

  ballY = (ballYdirPos) ? ballY + 1 : ballY - 1;

  // Blink ball position for losing side indication
  for (int i = 0; i < 20; i++)
  {
    LM.turnOn(ballY, ballX);
    delay(100);
  }

  // Display ending symbols or numbers based on a 'simple' flag
  for (int i = 0; i < 3; i++)
  {
    ShowSymbol('P');
    ShowSymbol((isP1) ? '1' : '2');
    ShowSymbol('W');
    ShowSymbol('I');
    ShowSymbol('N');
  }

  exit(1); // Exit code; reset to restart
}

// Function to change ball position
void ballchange()
{
  bool checkIsFacePos;
  if (balldelayed)
  {
    if (millis() - timeupdate > balldelaytime)
    {
      if (ballX == 1 || ballX == 6) // Check if ball reaches player edge
      {
        checkIsFacePos = (ballX == 1) ? false : true; // set the checking direction

        if (memory[ballY][ballX + checkIsFacePos] != 1) // Check if player edge blocks the ball
        {
          (checkIsFacePos) ? End(1) : End(2); // run ending respectively
        }
        else
        {
          ballXdirPos = (ballXdirPos) ? false : true; // Change X direction of the ball
        }
      }

      ballX = (ballXdirPos == true) ? ballX + 1 : ballX - 1; // shift th eball in X direction

      if (ballY == 0 || ballY == 7)
      {
        ballYdirPos = (ballYdirPos == true) ? false : true; // Change Y direction if ball reaches top or bottom
      }

      ballY = (ballYdirPos == true) ? ballY + 1 : ballY - 1; // shift the ball Y direction

      balldelayed = false;
    }
  }
  else
  {
    // Record previous time
    timeupdate = millis();
    balldelayed = true;
  }
}

// Setup function
void setup()
{
  // Initial symbols on LED Matrix
  ShowSymbol(N3, 1000);
  ShowSymbol(N2, 1000);
  ShowSymbol(N1, 1000);

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

#include <LEDMatrix.h>

// (Row/Column) pins must be put in ascending order, follow your datasheet.
int posPin[] = { 2, 3, 4, 5, 6, 7, 8, 9 };
int negPin[] = { 10, 11, 12, 13, A0, A1, A2, A3 };

// array to show specific pattern called "wall"
int wall[] = {0, 0, 1, 1, 1, 0, 0, 0};

LEDMatrix LM(posPin, 8, negPin, 8); // Initialization
/*
  Class name, Object_name(
  positive_pin_array[],
  size of positive_pin_array[],
  negative_pin_array[],
  size of positive_pin_array[]
  )
*/

// defining terms resembling pins to increase readability
#define P1left A4
#define P1right A5
#define P2left A6
#define P2right A7

// variables for time control
bool balldelay = false;
unsigned long timeupdate;
int balldelaytime = 1000;

// varibales for ball control
int ballX = random(3, 5);
int ballY = random(3, 5);
int ballXdirection = 1;
int ballYdirection = 1;
int check;
int mem;

// variables for input detection
int P1leftstat;
int P1rightstat;
int P2leftstat;
int P2rightstat;
int P1change = 0;
int P2change = 0;

// variables to alter array position, refer example 'display_basics.ino'
int P1shift = 0;
int P2shift = 0;

// allocate space for game memory
int memory[8][8];

// fixed symbols for display purposes
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

// function to display given symbol for set time
void displaywithtime(int Matrix[][8], int time = 1000)
{
  timeupdate = millis();
  while (millis() - timeupdate < time)
  {
    LM.Symbol(Matrix);
  }
};

int limitingshift(int value, bool change) // limiting the input of players
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
};

void fasterball()//increase ball speed
{
balldelaytime = ceil(500 - millis() / 150);
}

void checkbutton() // checking for button inputs
{
  P1leftstat = digitalRead(P1left); // reads
  P1rightstat = digitalRead(P1right);
  P2leftstat = digitalRead(P2left);
  P2rightstat = digitalRead(P2right);
  if (P1change == 0) // only change if the user has let go of the button
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
    if (P1leftstat == 0 && P1rightstat == 0) // when let go, renable the input
    {
      P1change = 0;
    }
  }
  if (P2change == 0) // same as P1
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
};

void display() // displaying on the matrix
{
  LM.customRow(wall, 0, P1shift);
  LM.customRow(wall, 7, P2shift);
  LM.turnOn(ballY, ballX);
};

void updateMem(bool normal = true) // updating the game memory
{
  for (int i = 0; i < 8; i++) // clear the momory
  {
    for (int j = 0; j < 8; j++)
    {
      memory[j][i] = 0;
    }
  }
  if (normal) // update the memory if needed
  {
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
  } // eitherwise just leave it blank to save space
};

void End(int result, bool simple = false) // ending the game.
{
  if (ballXdirection == 1) // final changes
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
  updateMem(false);            // clear memory
  for (int i = 0; i < 20; i++) // blink the ball position, showing the losing side
  {
    LM.turnOn(ballY, ballX);
    delay(100);
  }
  if (!simple) // displays symbol ending
  {
    for (int i = 0; i < 3; i++)
    {
      displaywithtime(P);
      if (result == 1)
      {
        displaywithtime(N1);
      }
      else if (result == 2)
      {
        displaywithtime(N2);
      }
      displaywithtime(W);
      displaywithtime(I);
      displaywithtime(N);
    }
  }
  else // displays simple number flash ending
  {
    for (int i = 0; i < 30; i++)
    {
      if (result == 1)
      {
        displaywithtime(N1, 300);
      }
      else if (result == 2)
      {
        displaywithtime(N2, 300);
      }
      delay(300);
    }
  }
  exit(1); // exit code, to restart, hit the reset button
};

void ballchange() // ball changes
{
  if (balldelay) // if the previous time was recorded
  {
    if (millis() - timeupdate > balldelaytime) // if the difference in time is sufficient
    {
      if (ballX == 1 || ballX == 6) // if on player border
      {
        if (ballX == 1)
        {
          check = -1;
        }
        else
        {
          check = 1;
        }
        if (memory[ballY][ballX + check] != 1) // if player edge doesnt block their end.
        {
          if (check == -1)
          {
            End(2, false); // calls End()
          }
          else if (check == 1)
          {
            End(1, false); // calls End()
          }
        }
        else // switch direction on edge
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
      if (ballXdirection == 1) // change ballX +/- 1
      {
        ballX++;
      }
      else
      {
        ballX--;
      }

      if (ballY == 0 || ballY == 7) // if on edge change direction
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
      if (ballYdirection == 1) // change ballY +/- 1
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
  else // record the prvious time
  {
    timeupdate = millis();
    balldelay = true;
  }
};

void setup()
{
  for (int i = 2; i < 6; i++) // set input pins
  {
    pinMode(i, INPUT);
  }
  displaywithtime(N3); // countdown
  displaywithtime(N2);
  displaywithtime(N1);
  for (int i = 0; i < 20; i++) // blink the ball position, showing the starting position
  {
    LM.turnOn(ballY, ballX);
    delay(100);
  }
};

void loop() // calling functions in sequence
{
  updateMem();
  display();
  checkbutton();
  ballchange();
  fasterball();
};

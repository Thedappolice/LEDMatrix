#include <LEDMatrix.h>

// Pin configurations for LED Matrix
const int POS_PIN[] = {2, 3, 4, 5, 6, 7, 8, 9};
const int NEG_PIN[] = {10, 11, 12, 13, A0, A1, A2, A3};

// Wall pattern array
const int WALL[] = {0, 0, 1, 1, 1, 0, 0, 0};

// LED Matrix object initialization
LEDMatrix LM(POS_PIN, 8, NEG_PIN, 8);

// Pins for the buttons
#define P1_LEFT A4
#define P1_RIGHT A5
#define P2_LEFT A7
#define P2_RIGHT A6


struct Player
{
  int shift;
  bool changed;
  int leftPin;
  int rightPin;
};

Player player1 = {0, false, P1_LEFT, P1_RIGHT};
Player player2 = {0, false, P2_LEFT, P2_RIGHT};

struct Ball
{
  int x;
  int y;
  bool xDirPos;
  bool yDirPos;
  unsigned long delayTime;
  unsigned long lastUpdateTime;
  bool delayed;
};

Ball ball = {random(3, 5), random(3, 5), random(0, 2) == 0, random(0, 2) == 0, 1000, 0, false};

int memory[8][8] = {{0}};

void ShowSymbol(char input, unsigned long delay = 0)
{
  // display arrays
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

  // Copy input symbol to display array
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

  // Call the LM.Symbol function to display the symbol on the LED matrix with time if given
  if (delay != 0)
  {
    LM.Symbol(display, delay);
  }
  else
  {
    LM.Symbol(display);
  }
};

int limitShift(int value, bool positiveDir)
{
  if (positiveDir && value < 3)
    return value + 1;
  if (!positiveDir && value > -2)
    return value - 1;
  return value;
}

void fasterBall(Ball &ball)
{
  ball.delayTime = max(500 - millis() / 150, 50);
}

void checkButton(Player &player)
{
  bool leftState = digitalRead(player.leftPin) == HIGH;
  bool rightState = digitalRead(player.rightPin) == HIGH;

  if (!player.changed)
  {
    if (leftState)
    {
      player.shift = limitShift(player.shift, false);
    }

    if (rightState)
    {
      player.shift = limitShift(player.shift, true);
    }

    player.changed = true;
  }
  else if (!leftState && !rightState)
  {
    player.changed = false;
  }
}

void updateMemory(Player &player1, Player &player2, Ball &ball)
{
  memset(memory, 0, sizeof(memory));

  for (int j = 2 + player1.shift; j <= 4 + player1.shift; j++)
  {
    if (j >= 0 && j < 8)
      memory[0][j] = 1;
  }
  for (int j = 2 + player2.shift; j <= 4 + player2.shift; j++)
  {
    if (j >= 0 && j < 8)
      memory[7][j] = 1;
  }

  memory[ball.y][ball.x] = 1;
}

void display(Player &player1, Player &player2, Ball &ball)
{
  LM.customRow(WALL, 0, player1.shift);
  LM.customRow(WALL, 7, player2.shift);
  LM.turnOn(ball.y, ball.x);
}

void updateBall(Ball &ball)
{
  if (ball.delayed)
  {
    if (millis() - ball.lastUpdateTime > ball.delayTime)
    {
      if (ball.y == 0 || ball.y == 7)
      {
        ball.yDirPos = !ball.yDirPos;
      }

      ball.y += ball.yDirPos ? 1 : -1;

      if (ball.x == 0 || ball.x == 7)
      {
        if (memory[ball.y][ball.x] != 1)
        {
          ball.x == 0 ? endGame(1) : endGame(2);
        }
        ball.xDirPos = !ball.xDirPos;
      }
      ball.x += ball.xDirPos ? 1 : -1;

      ball.delayed = false;
    }
  }
  else
  {
    ball.lastUpdateTime = millis();
    ball.delayed = true;
  }
}

void endGame(bool isP1)
{
  for (int i = 0; i < 20; i++)
  {
    LM.turnOn(ball.y, ball.x);
    delay(100);
  }
  for (int i = 0; i < 3; i++)
  {
    showSymbol('P');
    showSymbol(isP1 ? '1' : '2');
    showSymbol('W');
    showSymbol('I');
    showSymbol('N');
  }
  exit(1);
}

void setup()
{
  showSymbol('3', 1000);
  showSymbol('2', 1000);
  showSymbol('1', 1000);
  for (int i = 0; i < 20; i++)
  {
    LM.turnOn(ball.y, ball.x);
    delay(100);
  }
}

void loop()
{
  updateMemory(player1, player2, ball);
  display(player1, player2, ball);
  checkButton(player1);
  checkButton(player2);
  updateBall(ball);
  fasterBall(ball);
}

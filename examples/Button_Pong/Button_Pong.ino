#include <LEDMatrix.h>

int posPin[] = { 6, 7, 8, 9, 10, 11, 12, 13 };
int negPin[] = { A0, A1, A2, A3, A4, A5, A6, A7 };

int wall[] = { 0, 0, 0, 1, 1, 1, 0, 0 };

LEDMatrix LM(posPin, 8, negPin, 8);

#define P1left 4
#define P1right 5
#define P2left 2
#define P2right 3

int timeupdate;

int ballX;
int ballY;
int Xdirection = random(2);
int Ydirection = random(2);

int P1leftstat;
int P1rightstat;
int P2leftstat;
int P2rightstat;

int P1shift = 0;
int P2shift = 0;

int memory[8][8] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 0, 0, 0, 0, 0, 0, 1 },
  { 1, 0, 0, 0, 0, 0, 0, 1 },
  { 1, 0, 0, 0, 0, 0, 0, 1 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};

int W[8][8] = {
  { 1, 0, 0, 0, 0, 0, 0, 1 },
  { 1, 0, 0, 0, 0, 0, 0, 1 },
  { 1, 0, 0, 1, 1, 0, 0, 1 },
  { 1, 1, 0, 1, 1, 0, 1, 1 },
  { 1, 1, 0, 1, 1, 0, 1, 1 },
  { 1, 1, 1, 0, 0, 1, 1, 1 },
  { 0, 1, 1, 0, 0, 1, 1, 0 },
  { 0, 1, 0, 0, 0, 0, 1, 0 }
};

int I[8][8] = {
  { 0, 1, 1, 1, 1, 1, 1, 0 },
  { 0, 1, 1, 1, 1, 1, 1, 0 },
  { 0, 0, 0, 1, 1, 0, 0, 0 },
  { 0, 0, 0, 1, 1, 0, 0, 0 },
  { 0, 0, 0, 1, 1, 0, 0, 0 },
  { 0, 0, 0, 1, 1, 0, 0, 0 },
  { 0, 1, 1, 1, 1, 1, 1, 0 },
  { 0, 1, 1, 1, 1, 1, 1, 0 }
};

int N[8][8] = {
  { 1, 1, 0, 0, 0, 0, 1, 1 },
  { 1, 1, 1, 0, 0, 0, 1, 1 },
  { 1, 1, 1, 1, 0, 0, 1, 1 },
  { 1, 1, 1, 1, 1, 0, 1, 1 },
  { 1, 1, 0, 1, 1, 1, 1, 1 },
  { 1, 1, 0, 0, 1, 1, 1, 1 },
  { 1, 1, 0, 0, 0, 1, 1, 1 },
  { 1, 1, 0, 0, 0, 0, 1, 1 }
};

int P[8][8] = {
  { 0, 1, 1, 1, 1, 1, 0, 0 },
  { 0, 1, 1, 0, 0, 0, 1, 0 },
  { 0, 1, 1, 0, 0, 0, 1, 0 },
  { 0, 1, 1, 1, 1, 1, 0, 0 },
  { 0, 1, 1, 0, 0, 0, 0, 0 },
  { 0, 1, 1, 0, 0, 0, 0, 0 },
  { 0, 1, 1, 0, 0, 0, 0, 0 },
  { 0, 1, 1, 0, 0, 0, 0, 0 }
};

int P1[8][8] = {
  { 0, 0, 0, 1, 1, 0, 0, 0 },
  { 0, 0, 1, 1, 1, 0, 0, 0 },
  { 0, 1, 1, 1, 1, 0, 0, 0 },
  { 0, 0, 0, 1, 1, 0, 0, 0 },
  { 0, 0, 0, 1, 1, 0, 0, 0 },
  { 0, 0, 0, 1, 1, 0, 0, 0 },
  { 0, 1, 1, 1, 1, 1, 1, 0 },
  { 0, 1, 1, 1, 1, 1, 1, 0 }
};

int P2[8][8] = {
  { 0, 0, 1, 1, 1, 1, 0, 0 },
  { 0, 1, 1, 1, 1, 1, 1, 0 },
  { 0, 1, 1, 0, 0, 1, 1, 0 },
  { 0, 0, 0, 0, 1, 1, 1, 0 },
  { 0, 0, 0, 1, 1, 1, 0, 0 },
  { 0, 0, 1, 1, 1, 0, 0, 0 },
  { 0, 1, 1, 1, 1, 1, 1, 0 },
  { 0, 1, 1, 1, 1, 1, 1, 0 }
};

void displaywithtime(int Matrix[][8], int time = 1000) {
  timeupdate = millis();
  while (millis() - timeupdate < time) {
    LM.Symbol(Matrix);
  }
};

void checkbutton() {
  P1leftstat = digitalRead(P1left);
  P1rightstat = digitalRead(P1right);
  P2leftstat = digitalRead(P2left);
  P2rightstat = digitalRead(P2right);
};

void display() {
  LM.Symbol(memory);
  LM.turnOn(ballX, ballY);
};

void checklogic(int memory[][8]){

};

int limitingshift(int value, bool change) {
  int mem;
  if (value < 3 || change == true) {
    mem = value++;
  } else if (value > -4 || change == false) {
    mem = value--;
  }
  return mem;
};

int updateMem(int Matrix[][8])
{
  int newMatrix[8][8];
 for(int i = 0; i<8; i++)
 {
  for(int j = 0; j<8;j++)
  {
    newMatrix[j][i] = ;
  }
 }
 return newMatrix;
};

void End(int result, bool simple = false) {
  if (!simple) {
    for (int i = 0; i < 3; i++) {
      displaywithtime(P);
      if (result == 1) {
        displaywithtime(P1);
      } else if (result == 2) {
        displaywithtime(P2);
      }
      displaywithtime(W);
      displaywithtime(I);
      displaywithtime(N);
    }

  } else {
    for (int i = 0; i < 30; i++) {
      if (result == 1) {
        displaywithtime(P1, 300);
      } else if (result == 2) {
        displaywithtime(P2, 300);
      }
      delay(300);
    }
  }
  exit(1);
};

void setup() {
  for (int i = 2; i < 6; i++) {
    pinMode(i, INPUT);
  }
};

void loop() {
  displaywithtime(memory);
  // memory = updateMem(memory);
  // display();
  // checkbutton();
  // checklogic();
};

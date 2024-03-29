#include <LEDMatrix.h>

// (Row/Column) pins must be put in ascending order, follow your datasheet.
int posPin[] = { 2, 3, 4, 5, 6, 7, 8, 9 };
int negPin[] = { 10, 11, 12, 13, A0, A1, A2, A3 };

//"Z" named 2d matrix
int Z[8][8] = {
  { 0, 1, 1, 1, 1, 1, 1, 0 },
  { 0, 1, 1, 1, 1, 1, 1, 0 },
  { 0, 0, 0, 0, 1, 1, 1, 0 },
  { 0, 0, 0, 1, 1, 1, 0, 0 },
  { 0, 0, 1, 1, 1, 0, 0, 0 },
  { 0, 1, 1, 1, 0, 0, 0, 0 },
  { 0, 1, 1, 1, 1, 1, 1, 0 },
  { 0, 1, 1, 1, 1, 1, 1, 0 }
};
//"E"
int E[8][8] = {
  { 0, 1, 1, 1, 1, 1, 1, 0 },
  { 0, 1, 1, 1, 1, 1, 1, 0 },
  { 0, 1, 1, 0, 0, 0, 0, 0 },
  { 0, 1, 1, 1, 1, 1, 1, 0 },
  { 0, 1, 1, 1, 1, 1, 1, 0 },
  { 0, 1, 1, 0, 0, 0, 0, 0 },
  { 0, 1, 1, 1, 1, 1, 1, 0 },
  { 0, 1, 1, 1, 1, 1, 1, 0 }
};
//"N"
int N[8][8] = {
  { 0, 1, 1, 0, 0, 1, 1, 0 },
  { 0, 1, 1, 0, 0, 1, 1, 0 },
  { 0, 1, 1, 1, 0, 1, 1, 0 },
  { 0, 1, 1, 1, 1, 1, 1, 0 },
  { 0, 1, 1, 1, 1, 1, 1, 0 },
  { 0, 1, 1, 0, 1, 1, 1, 0 },
  { 0, 1, 1, 0, 0, 1, 1, 0 },
  { 0, 1, 1, 0, 0, 1, 1, 0 }
};
//"N"
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
//"U"
int U[8][8] = {
  { 0, 1, 1, 0, 0, 1, 1, 0 },
  { 0, 1, 1, 0, 0, 1, 1, 0 },
  { 0, 1, 1, 0, 0, 1, 1, 0 },
  { 0, 1, 1, 0, 0, 1, 1, 0 },
  { 0, 1, 1, 0, 0, 1, 1, 0 },
  { 0, 1, 1, 0, 0, 1, 1, 0 },
  { 0, 1, 1, 1, 1, 1, 1, 0 },
  { 0, 0, 1, 1, 1, 1, 0, 0 }
};
//"S"
int S[8][8] = {
  { 0, 0, 1, 1, 1, 1, 0, 0 },
  { 0, 1, 1, 1, 1, 1, 1, 0 },
  { 0, 1, 1, 0, 0, 0, 0, 0 },
  { 0, 1, 1, 1, 1, 1, 0, 0 },
  { 0, 0, 1, 1, 1, 1, 1, 0 },
  { 0, 0, 0, 0, 0, 1, 1, 0 },
  { 0, 1, 1, 1, 1, 1, 1, 0 },
  { 0, 0, 1, 1, 1, 1, 0, 0 }
};
//"End"
int End[8][8] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 1, 1, 0, 0, 0, 0, 1 },
  { 1, 0, 0, 0, 0, 0, 0, 1 },
  { 1, 1, 0, 0, 1, 0, 0, 1 },
  { 1, 0, 0, 1, 0, 1, 1, 0 },
  { 1, 1, 1, 1, 0, 1, 0, 1 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
};

LEDMatrix LM(posPin, 8, negPin, 8);  //Initialization
/*Class name, Object_name(
  positive_pin_array[],
  size of positive_pin_array[],
  negative_pin_array[],
  size of positive_pin_array[])
*/

void setup() {
}

void loop() {
  while (millis() / 1000 < 2) {
    LM.Symbol(Z);//Object_name.Symbol(name_of_character);
  }
  while (millis() / 1000 < 4) {
    LM.Symbol(E);
  }
  while (millis() / 1000 < 6) {
    LM.Symbol(N);
  }
  while (millis() / 1000 < 8) {
    LM.Symbol(I);
  }
  while (millis() / 1000 < 10) {
    LM.Symbol(U);
  }
  while (millis() / 1000 < 12) {
    LM.Symbol(S);
  }
  LM.Symbol(End);
}

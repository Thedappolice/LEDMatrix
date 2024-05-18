#include <LEDMatrix.h>

// Pin configurations
int posPin[] = {2, 3, 4, 5, 6, 7, 8, 9};
int negPin[] = {10, 11, 12, 13, A0, A1, A2, A3};
LEDMatrix LM(posPin, 8, negPin, 8);

// Joystick inputs
int JoyX;
int JoyY;

int length = 3;
int body[64][2] = {{0}}; // Assuming a maximum length of 64 for the body

bool foodExists = false;
int Mayfood[64][2] = {{0}}; // Assuming a maximum of 64 possible food locations

unsigned long timeupdate;

// End and number matrices
int End[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 0, 0, 1, 0, 0, 1},
    {1, 0, 0, 1, 0, 1, 1, 0},
    {1, 1, 1, 1, 0, 1, 0, 1},
    {0, 0, 0, 0, 0, 0, 0, 0}
};
int N1[8][8] = {
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 0, 0},
    {0, 1, 1, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 0}
};
int N2[8][8] = {
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 0, 0, 1, 1, 0},
    {0, 0, 0, 0, 1, 1, 1, 0},
    {0, 0, 0, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 0}
};
int N3[8][8] = {
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 0, 0, 0, 1, 1, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 0, 0, 0, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 1, 1, 1, 1, 0, 0}
};

int display[8][8] = {{0}};
int memory[8][8][3] = {
    {{0, 0, 0}, {0, 1, 0}, {0, 2, 0}, {0, 3, 0}, {0, 4, 0}, {0, 5, 0}, {0, 6, 0}, {0, 7, 0}},
    {{1, 0, 0}, {1, 1, 0}, {1, 2, 0}, {1, 3, 0}, {1, 4, 0}, {1, 5, 0}, {1, 6, 0}, {1, 7, 0}},
    {{2, 0, 0}, {2, 1, 0}, {2, 2, 0}, {2, 3, 0}, {2, 4, 0}, {2, 5, 0}, {2, 6, 0}, {2, 7, 0}},
    {{3, 0, 0}, {3, 1, 0}, {3, 2, 0}, {3, 3, 0}, {3, 4, 0}, {3, 5, 0}, {3, 6, 0}, {3, 7, 0}},
    {{4, 0, 0}, {4, 1, 0}, {4, 2, 0}, {4, 3, 0}, {4, 4, 1}, {4, 5, 0}, {4, 6, 0}, {4, 7, 0}},
    {{5, 0, 0}, {5, 1, 0}, {5, 2, 0}, {5, 3, 0}, {5, 4, 2}, {5, 5, 0}, {5, 6, 0}, {5, 7, 0}},
    {{6, 0, 0}, {6, 1, 0}, {6, 2, 0}, {6, 3, 0}, {6, 4, 3}, {6, 5, 0}, {6, 6, 0}, {6, 7, 0}},
    {{7, 0, 0}, {7, 1, 0}, {7, 2, 0}, {7, 3, 0}, {7, 4, 0}, {7, 5, 0}, {7, 6, 0}, {7, 7, 0}}
};

char direction = 'u';
bool end = false;

void displaywithtime(int Matrix[][8], int time = 1000) {
    timeupdate = millis();
    while (millis() - timeupdate < time) {
        LM.Symbol(Matrix);
    }
}

void checkdirection() {
    char JYSTCK = direction; // Default to current direction
    if (analogRead(A7) > 768) {
        JYSTCK = 'l';
    } else if (analogRead(A7) < 256) {
        JYSTCK = 'r';
    } else if (analogRead(A6) > 768) {
        JYSTCK = 'd';
    } else if (analogRead(A6) < 256) {
        JYSTCK = 'u';
    }

    char oppdirection;
    switch (direction) {
        case 'u': oppdirection = 'd'; break;
        case 'd': oppdirection = 'u'; break;
        case 'l': oppdirection = 'r'; break;
        case 'r': oppdirection = 'l'; break;
    }

    if (JYSTCK != oppdirection) {
        direction = JYSTCK;
    }
}

void generateFood() {
    if (!foodExists) {
        int count = 0;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if(memory[i][j][2] == 0) { Mayfood[count][0] = i; Mayfood[count][1] = j; count++; } } } if (count > 0) { int randIndex = random(0, count); memory[Mayfood[randIndex][0]][Mayfood[randIndex][1]][2] = -1; foodExists = true; } } }void refreshMem() { int count = 0; for (int c = 0; c < length; c++) { for (int i = 0; i < 8; i++) { for (int j = 0; j < 8; j++) { if (memory[i][j][2] == length - c) { body[count][0] = i; body[count][1] = j; count++; } } } } }void MemtoDisplay() { for (int i = 0; i < 8; i++) { for (int j = 0; j < 8; j++) { if (memory[i][j][2] != 0) { display[i][j] = 1; } else { display[i][j] = 0; } } } }void setup() { Serial.begin(9600); MemtoDisplay(); displaywithtime(N3); // countdown displaywithtime(N2); displaywithtime(N1); for (int i = 0; i < 20; i++) { // blink the ball position, showing the starting position LM.Symbol(display); delay(100); } }void loop() { while (!end) { generateFood(); checkdirection(); refreshMem(); MemtoDisplay(); LM.Symbol(display); } }
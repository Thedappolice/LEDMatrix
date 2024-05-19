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
int head[2] = {4, 4};

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
    {0, 0, 0, 0, 0, 0, 0, 0}};

int Win1[8][8] = {
    {0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 0, 0, 0},
    {0, 1, 1, 0, 1, 1, 0, 0},
    {1, 1, 0, 0, 0, 1, 1, 0}};
int Win2[8][8] = {
    {0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 1, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 1},
    {0, 0, 1, 1, 1, 1, 1, 0},
    {0, 0, 0, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 0, 1, 1, 0},
    {0, 1, 1, 0, 0, 0, 1, 1}};

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

int display[8][8] = {{0}};

int memory[8][8][3] = {
    {{0, 0, 0}, {0, 1, 0}, {0, 2, 0}, {0, 3, 0}, {0, 4, 0}, {0, 5, 0}, {0, 6, 0}, {0, 7, 0}},
    {{1, 0, 0}, {1, 1, 0}, {1, 2, 0}, {1, 3, 0}, {1, 4, 0}, {1, 5, 0}, {1, 6, 0}, {1, 7, 0}},
    {{2, 0, 0}, {2, 1, 0}, {2, 2, 0}, {2, 3, 0}, {2, 4, 0}, {2, 5, 0}, {2, 6, 0}, {2, 7, 0}},
    {{3, 0, 0}, {3, 1, 0}, {3, 2, 0}, {3, 3, 0}, {3, 4, 0}, {3, 5, 0}, {3, 6, 0}, {3, 7, 0}},
    {{4, 0, 0}, {4, 1, 0}, {4, 2, 0}, {4, 3, 0}, {4, 4, 1}, {4, 5, 0}, {4, 6, 0}, {4, 7, 0}},
    {{5, 0, 0}, {5, 1, 0}, {5, 2, 0}, {5, 3, 0}, {5, 4, 2}, {5, 5, 0}, {5, 6, 0}, {5, 7, 0}},
    {{6, 0, 0}, {6, 1, 0}, {6, 2, 0}, {6, 3, 0}, {6, 4, 3}, {6, 5, 0}, {6, 6, 0}, {6, 7, 0}},
    {{7, 0, 0}, {7, 1, 0}, {7, 2, 0}, {7, 3, 0}, {7, 4, 0}, {7, 5, 0}, {7, 6, 0}, {7, 7, 0}}};

char direction = 'u';
bool end = false;
bool win = false;

void displaywithtime(int Matrix[][8], int time = 1000)
{
    timeupdate = millis();
    while (millis() - timeupdate < time)
    {
        LM.Symbol(Matrix);
    }
};

void checkdirection()
{
    static unsigned long lastDebounceTime = 0;
    unsigned long debounceDelay = 50; // 50 ms debounce delay
    char JYSTCK = direction;          // Default to current direction

    if (millis() - lastDebounceTime > debounceDelay)
    {
        if (analogRead(A7) > 768)
        {
            JYSTCK = 'l';
        }
        else if (analogRead(A7) < 256)
        {
            JYSTCK = 'r';
        }
        else if (analogRead(A6) > 768)
        {
            JYSTCK = 'd';
        }
        else if (analogRead(A6) < 256)
        {
            JYSTCK = 'u';
        }

        char oppdirection;
        switch (direction)
        {
        case 'u':
            oppdirection = 'd';
            break;
        case 'd':
            oppdirection = 'u';
            break;
        case 'l':
            oppdirection = 'r';
            break;
        case 'r':
            oppdirection = 'l';
            break;
        }

        if (JYSTCK != oppdirection)
        {
            direction = JYSTCK;
            lastDebounceTime = millis(); // Update debounce time
        }
    }
}

void generateFood()
{
    if (!foodExists)
    {
        int count = 0;

        // Iterate through the board to find empty spots
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (memory[i][j][2] == 0)
                { // Check for empty spot
                    Mayfood[count][0] = i;
                    Mayfood[count][1] = j;
                    count++;
                }
            }
        }

        // If there are empty spots, place food
        if (count > 0)
        {
            int randIndex = (int)random(0, count);
            memory[Mayfood[randIndex][0]][Mayfood[randIndex][1]][2] = -1;
            foodExists = true;
        }
        else
        {
            end = true;
            win = true;
            return;
        }
    }
}

void refreshMem()
{
    int prelength = length;
    bool eat = false;
    // Update head position based on direction
    switch (direction)
    {
    case 'u':
        if (head[0] - 1 < 0)
        {
            end = true;
            return;
        }
        else
        {
            head[0] = head[0] - 1;
        }
        break;
    case 'd':
        if (head[0] + 1 > 7)
        {
            end = true;
            return;
        }
        else
        {
            head[0] = head[0] + 1;
        }
        break;
    case 'l':
        if (head[1] - 1 < 0)
        {
            end = true;
            return;
        }
        else
        {
            head[1] = head[1] - 1;
        }
        break;
    case 'r':
        if (head[1] + 1 > 7)
        {
            end = true;
            return;
        }
        else
        {
            head[1] = head[1] + 1;
        }
        break;
    }

    // Check if the snake hits itself
    for (int i = 0; i < length - 1; i++)
    {
        if (body[i][0] == head[0] && body[i][1] == head[1])
        {
            end = true;
            return;
        }
    }

    // Check if the snake eats the food
    if (memory[head[0]][head[1]][2] == -1)
    {
        eat = true;
        prelength++;
        foodExists = false;
    }

    for (int i = length - 1; i > 0; i--)
    {
        body[i][0] = body[i - 1][0];
        body[i][1] = body[i - 1][1];
    }
    body[0][0] = head[0];
    body[0][1] = head[1];

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (memory[i][j][2] > 0)
            {
                memory[i][j][2] = 0;
            }
        }
    }

    memory[head[0]][head[1]][2] = 1;

    for (int i = 0; i < length - 1; i++)
    {
        memory[body[i + 1][0]][body[i + 1][1]][2] = i + 2;
    }

    length = prelength;
};

void MemtoDisplay()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (memory[i][j][2] != 0)
            {
                display[i][j] = 1;
            }
            else
            {
                display[i][j] = 0;
            }
        }
    }
};

void ending()
{
    for (int i = 0; i < 10; i++)
    {
        LM.Symbol(display);
        delay(100);
    }

    for (int c = length; c > 0; c--)
    {
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (memory[i][j][2] == c)
                {
                    memory[i][j][2] = 0;
                    MemtoDisplay();
                    displaywithtime(display, 250);
                }
            }
        }
    }
    if (!win)
    {
        displaywithtime(End, 10000);
    }
    else
    {
        for (int i = 0; i < 5; i++)
        {
            displaywithtime(Win1, 500);
            displaywithtime(Win2, 500);
        }
    }
    exit(0);
};

void setup()
{
    body[0][0] = 4;
    body[0][1] = 4;
    body[1][0] = 5;
    body[1][1] = 4;
    body[2][0] = 6;
    body[2][1] = 4;

    MemtoDisplay();
    displaywithtime(N3); // Countdown
    displaywithtime(N2);
    displaywithtime(N1);
    for (int i = 0; i < 20; i++)
    {
        LM.Symbol(display);
        delay(100);
    }
}

void loop()
{
    unsigned long currentMillis = millis();
    static unsigned long previousMillis = 0;
    unsigned long interval = 500; // Refresh interval in milliseconds

    if (currentMillis - previousMillis >= interval)
    {
        previousMillis = currentMillis;
        if (!end)
        {
            // generateFood();
            checkdirection();
            refreshMem();
            MemtoDisplay();
            displaywithtime(display, interval);
        }
        else
        {
            ending();
        }
    }
}
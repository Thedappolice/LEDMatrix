#include "Joystick_snake.h"

// Constructor
Joystick_snake::Joystick_snake(LEDMatrix<GRID_SIZE, GRID_SIZE> &displayMatrix)
{
    LM = &displayMatrix;
}

// Destructor
Joystick_snake::~Joystick_snake()
{
}

// Main Game Loop
void Joystick_snake::gameLoop()
{
    if (!end)
    {
        generateFood();
        checkInput();
        updateSnake();
        displayGame();
    }
    else
    {
        endGame();
    }
}

// Check User Input (Joystick)
void Joystick_snake::checkInput()
{
    char Command = direction;
    char oppDir = 'u';

    if (analogRead(A7) > 768)
    {
        Command = 'l';
        oppDir = 'r';
    }
    if (analogRead(A7) < 256)
    {
        Command = 'r';
        oppDir = 'l';
    }
    if (analogRead(A6) > 768)
    {
        Command = 'd';
        oppDir = 'u';
    }
    if (analogRead(A6) < 256)
    {
        Command = 'u';
        oppDir = 'd';
    }

    if (Command != oppDir)
        direction = Command;
}

// Generate Food
void Joystick_snake::generateFood()
{
    if (!foodExists)
    {
        int emptyCells[MAX_LENGTH][2];
        int count = 0;

        for (int i = 0; i < GRID_SIZE; i++)
        {
            for (int j = 0; j < GRID_SIZE; j++)
            {
                if (memory[i][j] == 0)
                {
                    emptyCells[count][0] = i;
                    emptyCells[count][1] = j;
                    count++;
                }
            }
        }

        if (count > 0)
        {
            int randIndex = random(0, count);
            memory[emptyCells[randIndex][0]][emptyCells[randIndex][1]] = -1;
            foodExists = true;
        }
        else
        {
            end = true;
            win = true;
        }
    }
}

// Update Snake Position
void Joystick_snake::updateSnake()
{
    switch (direction)
    {
    case 'u':
        head[0] = (head[0] - 1 < 0) ? (end = true, head[0]) : head[0] - 1;
        break;
    case 'd':
        head[0] = (head[0] + 1 >= GRID_SIZE) ? (end = true, head[0]) : head[0] + 1;
        break;
    case 'l':
        head[1] = (head[1] - 1 < 0) ? (end = true, head[1]) : head[1] - 1;
        break;
    case 'r':
        head[1] = (head[1] + 1 >= GRID_SIZE) ? (end = true, head[1]) : head[1] + 1;
        break;
    }

    for (int i = 0; i < length; i++)
    {
        if (body[i][0] == head[0] && body[i][1] == head[1])
        {
            end = true;
            return;
        }
    }

    if (memory[head[0]][head[1]] == -1)
    {
        length++;
        foodExists = false;
    }

    for (int i = length - 1; i > 0; i--)
    {
        body[i][0] = body[i - 1][0];
        body[i][1] = body[i - 1][1];
    }

    body[0][0] = head[0];
    body[0][1] = head[1];

    memset(memory, 0, sizeof(memory));

    memory[head[0]][head[1]] = 1;
    for (int i = 0; i < length; i++)
    {
        memory[body[i][0]][body[i][1]] = 1;
    }
}

// Display the Game
void Joystick_snake::displayGame()
{
    LM->Symbol(memory);
}

// Show Symbol (For Countdown, End Game)
void Joystick_snake::showSymbol(char input, unsigned long duration)
{
    int display[8][8] = {0};

    int End[8][8] = {
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 0, 0, 1, 0, 0, 1},
        {1, 0, 0, 1, 0, 1, 1, 0},
        {1, 1, 1, 1, 0, 1, 0, 1},
        {0, 0, 0, 0, 0, 0, 0, 0}};

    if (input == 'E')
        memcpy(display, End, sizeof(display));

    LM->Symbol(display, duration);
}

// End Game Animation
void Joystick_snake::endGame()
{
    for (int i = 0; i < 10; i++)
    {
        LM->Symbol(memory);
        delay(100);
    }

    for (int i = length - 1; i >= 0; i--)
    {
        memory[body[i][0]][body[i][1]] = 0;
        LM->Symbol(memory, 250);
    }

    showSymbol('E', 10000);
}

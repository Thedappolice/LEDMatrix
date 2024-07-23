#include <LEDMatrix.h>

// Pin configurations
int posPin[] = {2, 3, 4, 5, 6, 7, 8, 9};
int negPin[] = {10, 11, 12, 13, A0, A1, A2, A3};

// Initialize LEDMatrix instance
LEDMatrix LM(posPin, 8, negPin, 8);

unsigned long interval = 500; // Refresh interval in milliseconds

// Snake initial positions and state
int length = 3;
int body[64][2] = {
    {4, 4},
    {5, 4},
    {6, 4},
};
int head[2] = {4, 4};

bool foodExists = false;

// Memory to track snake and food positions
int memory[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 2, 0, 0, 0},
    {0, 0, 0, 0, 3, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}};

char direction = 'u'; // Initial direction ('u' for up)
bool end = false;     // Game end condition
bool win = false;     // Game win flag

void ShowSymbol(char input, unsigned long duration = 0)
{
    int display[8][8] = {{0}};
    int End[8][8] = {
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 0, 0, 1, 0, 0, 1},
        {1, 0, 0, 1, 0, 1, 1, 0},
        {1, 1, 1, 1, 0, 1, 0, 1},
        {0, 0, 0, 0, 0, 0, 0, 0}};
    int Star[8][8] = {
        {0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 1, 1, 1, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 0},
        {0, 1, 1, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 0, 0, 0},
        {0, 1, 1, 0, 1, 1, 0, 0},
        {1, 1, 0, 0, 0, 1, 1, 0}};
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
    case '1':
        memcpy(display, N1, 8 * 8 * sizeof(int));
        break;
    case '2':
        memcpy(display, N2, 8 * 8 * sizeof(int));
        break;
    case '3':
        memcpy(display, N3, 8 * 8 * sizeof(int));
        break;
    case 'S':
        memcpy(display, Star, 8 * 8 * sizeof(int));
        break;
    case 'E':
        memcpy(display, End, 8 * 8 * sizeof(int));
        break;
    }

    (duration > 0) ? LM.Symbol(display, duration) : LM.Symbol(display);
};

// Function to check user input direction
void checkdInput()
{
    char Command = direction; // Default to current direction
    if (millis() - lastDebounceTime > debounceDelay)
    {
        char oppDir = 'u'; // Initialize opposite direction with default value

        if (analogRead(A7) > 768)
        {
            Command = 'l'; // Left
            oppDir = 'r';
        }
        else if (analogRead(A7) < 256)
        {
            Command = 'r'; // Right
            oppDir = 'l';
        }
        else if (analogRead(A6) > 768)
        {
            Command = 'd'; // Down
            oppDir = 'u';
        }
        else if (analogRead(A6) < 256)
        {
            Command = 'u'; // Up
            oppDir = 'd';
        }

        // Update direction if it's not opposite to the current direction
        if (Command != oppDir)
        {
            direction = Command;
        }
    }
};

// Function to generate food at random empty locations on the board
void generateFood()
{
    if (!foodExists)
    {
        int count = 0;
        int Mayfood[64][2] = {{0}};
        // Iterate through the board to find empty spots for food placement
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (memory[i][j] == 0)
                {
                    // Found an empty spot
                    Mayfood[count][0] = i;
                    Mayfood[count][1] = j;
                    count++;
                }
            }
        }

        // If there are empty spots, randomly place food
        if (count > 0)
        {
            int randIndex = random(0, count);
            memory[Mayfood[randIndex][0]][Mayfood[randIndex][1]] = -1; // Set food at this location
            foodExists = true;
        }
        else
        {
            // No empty spots left, game ends
            end = true;
            win = true; // This could be reconsidered depending on game logic
            return;
        }
    }
};

// Function to update snake's position and check for collisions
void refreshSnake()
{
    // Update head position based on current direction
    switch (direction)
    {
    case 'u':
        if (head[0] - 1 < 0)
        {
            end = true; // Hit top wall, game over
            return;
        }
        else
        {
            head[0] -= 1;
        }
        break;
    case 'd':
        if (head[0] + 1 > 7)
        {
            end = true; // Hit bottom wall, game over
            return;
        }
        else
        {
            head[0] += 1;
        }
        break;
    case 'l':
        if (head[1] - 1 < 0)
        {
            end = true; // Hit left wall, game over
            return;
        }
        else
        {
            head[1] -= 1;
        }
        break;
    case 'r':
        if (head[1] + 1 > 7)
        {
            end = true; // Hit right wall, game over
            return;
        }
        else
        {
            head[1] += 1;
        }
        break;
    }

    // Check if snake hits itself
    for (int i = 0; i < length; i++)
    {
        if (body[i][0] == head[0] && body[i][1] == head[1])
        {
            end = true; // Snake collided with itself, game over
            return;
        }
    }

    // Check if snake eats the food
    if (memory[head[0]][head[1]] == -1)
    {
        length++;           // Increase snake length immediately
        foodExists = false; // Food eaten, generate new food
    }

    // Shift body positions back one space
    for (int i = length - 1; i > 0; i--)
    {
        body[i][0] = body[i - 1][0];
        body[i][1] = body[i - 1][1];
    }

    // Update body with new head position
    body[0][0] = head[0];
    body[0][1] = head[1];

    // Clear memory
    memset(memory, 0, 8 * 8 * sizeof(int)); // Clear the entire memory array

    // Set new head position in memory
    memory[head[0]][head[1]] = 1;

    // Update memory with body positions
    for (int i = 0; i < length; i++)
    {
        memory[body[i][0]][body[i][1]] = 1;
    }
};

// Function to update display based on memory
void display(unsigned long duration)
{
    (duration > 0) ? LM.Symbol(memory, duration) : LM.Symbol(memory);
};

// Function to handle end of game animation
void ending()
{
    // Blink display to indicate end of game
    for (int i = 0; i < 10; i++)
    {
        LM.Symbol(memory);
        delay(100);
    }

    for (int c = length; c > 0; c--)
    {
        memory[body[c][0]][body[c][1]][2] = 0;
        LM.Symbol(memory, 250);
    }

    // Display end game message
    if (!win)
    {
        LM.Symbol(End, 10000); // Loss animation
        ShowSymbol('E');
    }
    else
    {
        for (int i = 0; i < 5; i++)
        {
            LM.Symbol('S', 500); // Win animation (blink)
        }
    }

    exit(0); // Exit the program after animation
};

void setup()
{
    // Initialize LEDMatrix with countdown
    ShowSymbol('3'); // Countdown 3
    ShowSymbol('2'); // Countdown 2
    ShowSymbol('1'); // Countdown 1

    randomSeed(analogRead(A4)); // Initialize random seed

    // Blink starting animation
    for (int i = 0; i < 10; i++)
    {
        LM.Symbol(memory);
        delay(200);
    }
}

void loop()
{
    // Game loop
    if (!end)
    {
        generateFood(); // Generate new food if needed
        checkdInput();  // Check user input direction
        refreshSnake(); // Update snake position and check collisions
        display();      // Update display based on memory
    }
    else
    {
        ending(); // Handle end of game
    }
}

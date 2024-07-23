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
int Mayfood[64][2] = {{0}};
// Assuming a maximum of 64 possible food locations

void ShowSymbol(char input, unsigned long duration = 0)
{
int display [8][8] ={{0}};
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
{case '1':
memcpy(display, N1, 8*8*sizeof(int));
break;
case '2':
memcpy(display, N2, 8*8*sizeof(int));
break;
case '3':
memcpy(display, N3, 8*8*sizeof(int));
break;
case 'S':
memcpy(display, Star, 8*8*sizeof(int));
break;
case 'E':
memcpy(display, End, 8*8*sizeof(int));
break;
}

(duration> 0)? LM.Symbol(display, duration) : LM.Symbol(display);
};

int display[8][8] = {{0}};

// Memory to track snake and food positions
int memory[8][8][3] = {
    {{0, 0, 0}, {0, 1, 0}, {0, 2, 0}, {0, 3, 0}, {0, 4, 0}, {0, 5, 0}, {0, 6, 0}, {0, 7, 0}},
    {{1, 0, 0}, {1, 1, 0}, {1, 2, 0}, {1, 3, 0}, {1, 4, 0}, {1, 5, 0}, {1, 6, 0}, {1, 7, 0}},
    {{2, 0, 0}, {2, 1, 0}, {2, 2, 0}, {2, 3, 0}, {2, 4, 0}, {2, 5, 0}, {2, 6, 0}, {2, 7, 0}},
    {{3, 0, 0}, {3, 1, 0}, {3, 2, 0}, {3, 3, 0}, {3, 4, 0}, {3, 5, 0}, {3, 6, 0}, {3, 7, 0}},
    {{4, 0, 0}, {4, 1, 0}, {4, 2, 0}, {4, 3, 0}, {4, 4, 1}, {4, 5, 0}, {4, 6, 0}, {4, 7, 0}},
    {{5, 0, 0}, {5, 1, 0}, {5, 2, 0}, {5, 3, 0}, {5, 4, 2}, {5, 5, 0}, {5, 6, 0}, {5, 7, 0}},
    {{6, 0, 0}, {6, 1, 0}, {6, 2, 0}, {6, 3, 0}, {6, 4, 3}, {6, 5, 0}, {6, 6, 0}, {6, 7, 0}},
    {{7, 0, 0}, {7, 1, 0}, {7, 2, 0}, {7, 3, 0}, {7, 4, 0}, {7, 5, 0}, {7, 6, 0}, {7, 7, 0}}};

char direction = 'u'; // Initial direction ('u' for up)
bool end = false;     // Game end flag
bool win = false;     // Game win flag

// Function to check user input direction
void checkdirection()
{
    char Command = direction; // Default to current direction
    if (millis() - lastDebounceTime > debounceDelay)
    {
        if (analogRead(A7) > 768)
        {
            Command = 'l'; // Left
        }
        else if (analogRead(A7) < 256)
        {
            Command = 'r'; // Right
        }
        else if (analogRead(A6) > 768)
        {
            Command = 'd'; // Down
        }
        else if (analogRead(A6) < 256)
        {
            Command = 'u'; // Up
        }

        char oppdirection = 'u'; // Initialize opposite direction with default value

        // Determine opposite direction based on current direction
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
        default:
            oppdirection = 'u'; // Default case to ensure oppdirection is always initialized
            break;
        }

        // Update direction if it's not opposite to the current direction
        if (Command != oppdirection)
        {
            direction = Command;
        }
    }
}

// Function to generate food at random empty locations on the board
void generateFood()
{
    if (!foodExists)
    {
        // Reset all potential food locations
        for (int i = 0; i < 64; i++)
        {
            Mayfood[i][0] = 0;
            Mayfood[i][1] = 0;
        }

        int count = 0;

        // Iterate through the board to find empty spots for food placement
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (memory[i][j][2] == 0)
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
            memory[Mayfood[randIndex][0]][Mayfood[randIndex][1]][2] = -1; // Set food at this location
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
}

// Function to update snake's position and check for collisions
void refreshMem()
{
    int prelength = length;

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
            head[0] = head[0] - 1;
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
            head[0] = head[0] + 1;
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
            head[1] = head[1] - 1;
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
            head[1] = head[1] + 1;
        }
        break;
    }

    // Check if snake hits itself
    for (int i = 0; i < length - 1; i++)
    {
        if (body[i][0] == head[0] && body[i][1] == head[1])
        {
            end = true; // Snake collided with itself, game over
            return;
        }
    }

    // Check if snake eats the food
    if (memory[head[0]][head[1]][2] == -1)
    {
        prelength++;        // Increase snake length
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
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (memory[i][j][2] > 0)
            {
                memory[i][j][2] = 0; // Clear previous positions
            }
        }
    }

    memory[head[0]][head[1]][2] = 1; // Set new head position in memory

    // Update memory with body positions
    for (int i = 0; i < length - 1; i++)
    {
        memory[body[i + 1][0]][body[i + 1][1]][2] = i + 2;
    }

    length = prelength; // Update snake length
}

// Function to update display based on memory
void MemtoDisplay()
{
    // Update display based on memory state
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (memory[i][j][2] != 0)
            {
                display[i][j] = 1; // Snake or food present at this position
            }
            else
            {
                display[i][j] = 0; // Empty space
            }
        }
    }
}

// Function to handle end of game animation
void ending()
{
    // Blink display to indicate end of game
    for (int i = 0; i < 10; i++)
    {
        LM.Symbol(display);
        delay(100);
    }

    // Retract snake from tail to head
    for (int c = length; c > 0; c--)
    {
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (memory[i][j][2] == c)
                {
                    memory[i][j][2] = 0;     // Clear snake position
                    MemtoDisplay();          // Update display
                    LM.Symbol(display, 250); // Display updated state
                }
            }
        }
    }

    // Display end game message
    if (!win)
    {
        LM.Symbol(End, 10000); // Loss animation
    }
    else
    {
        for (int i = 0; i < 5; i++)
        {
            LM.Symbol(Win, 500); // Win animation (blink)
        }
    }

    exit(0); // Exit the program after animation
}

void setup()
{
    // Initialize LEDMatrix with countdown
    LM.Symbol(N3); // Countdown 3
    LM.Symbol(N2); // Countdown 2
    LM.Symbol(N1); // Countdown 1

    randomSeed(analogRead(A4)); // Initialize random seed

    MemtoDisplay(); // Initialize display

    // Blink starting animation
    for (int i = 0; i < 10; i++)
    {
        LM.Symbol(display);
        delay(200);
    }
}

void loop()
{
    // Game loop
    if (!end)
    {
        generateFood();               // Generate new food if needed
        checkdirection();             // Check user input direction
        refreshMem();                 // Update snake position and check collisions
        MemtoDisplay();               // Update display based on memory
        LM.Symbol(display, interval); // Display updated state with interval delay
    }
    else
    {
        ending(); // Handle end of game
    }
}

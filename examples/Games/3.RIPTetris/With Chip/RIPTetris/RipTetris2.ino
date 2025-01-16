#include <math.h>

// Pin Definitions
#define ROTATE_PIN 31
#define LEFT_PIN 32
#define RIGHT_PIN 33
#define DOWN_PIN 30
#define RESET_PIN 34

// Grid Constants
#define GRID_WIDTH 8                      // Number of columns in the grid
#define GRID_HEIGHT (GRID_WIDTH * 2)      // Total grid height
#define CHECKING_HEIGHT (GRID_HEIGHT + 2) // Includes extra rows for checking overflows

#include <LEDMatrix.h>

// Pin configurations of 1st Led matrix
int posPintop[] = {37, 13, 16, 40, 23, 17, 22, 19};
int negPintop[] = {41, 21, 20, 38, 18, 39, 14, 15};
// Pin configurations of 2nd Led matrix
int posPinbot[] = {9, 4, 29, 6, 10, 28, 11, 26};
int negPinbot[] = {5, 12, 25, 8, 27, 7, 3, 2};
// Initialize LEDMatrix instance
LEDMatrix<8, 8> LMtop(posPintop, negPintop);
LEDMatrix<8, 8> LMbot(posPinbot, negPinbot);

// Game Data Structures
typedef struct
{
    int shape[4][2];       // Relative positions of blocks in the shape
    int coordinates[4][2]; // Absolute grid positions of blocks
    bool active;           // If the shape is currently active
} Tetrimino;

// Global Variables
Tetrimino currentShape = {0};                         // Current active Tetrimino
int stableMemory[CHECKING_HEIGHT][GRID_WIDTH] = {0};  // Stable grid (occupied blocks)
int displayMemory[CHECKING_HEIGHT][GRID_WIDTH] = {0}; // Display grid (includes active shapes)
int topLM[GRID_WIDTH][GRID_WIDTH] = {0};              // Top LED matrix representation
int botLM[GRID_WIDTH][GRID_WIDTH] = {0};              // Bottom LED matrix representation
unsigned long lastInputTime = 0;                      // Last time an input was processed
unsigned long inputInterval = 100;                    // Input polling interval in ms
unsigned long prev = 0;                               // Previous time for game interval
unsigned long interval = 500;                         // Default interval for block movement
int score = 0;                                        // Game score
bool end = false;                                     // Game end flag
bool ended = false;                                   // Animation completed flag

// Function Prototypes
void clearRow(int row);
void shiftRowsDown(int startRow);
void scanAndClearGrid();
void handleButtonPress(int buttonPin, void (*action)());
void gatherAndDisplay();
void sendScore(int score);
void genShape();
void alterShape(int mode);
void moveLeft();
void moveRight();
void moveDown();
void rotateShape();
void showEndAnimation();
void setup();
void loop();

/// --- Function Definitions ---

void alterShape(int mode)
{
    int newCoordinates[4][2];

    // Calculate the new position
    for (int i = 0; i < 4; i++)
    {
        newCoordinates[i][0] = currentShape.coordinates[i][0];
        newCoordinates[i][1] = currentShape.coordinates[i][1];

        if (mode == -1)
        { // Move left
            newCoordinates[i][1] -= 1;
        }
        else if (mode == 1)
        { // Move right
            newCoordinates[i][1] += 1;
        }
        else if (mode == 0)
        { // Move down
            newCoordinates[i][0] += 1;
        }
        else if (mode == 3)
        { // Rotate
            int y = currentShape.coordinates[0][0];
            int x = currentShape.coordinates[0][1];
            newCoordinates[i][0] = y - (currentShape.coordinates[i][1] - x);
            newCoordinates[i][1] = x + (currentShape.coordinates[i][0] - y);
        }
    }

    // Check for collisions
    bool validMove = true;
    for (int i = 0; i < 4; i++)
    {
        int y = newCoordinates[i][0];
        int x = newCoordinates[i][1];

        if (y >= CHECKING_HEIGHT || x < 0 || x >= GRID_WIDTH || stableMemory[y][x])
        {
            validMove = false;
            break;
        }
    }

    // Apply the new position if valid
    if (validMove)
    {
        memcpy(currentShape.coordinates, newCoordinates, sizeof(newCoordinates));
        if (mode == 0)
        { // Check for locking
            for (int i = 0; i < 4; i++)
            {
                if (currentShape.coordinates[i][0] == CHECKING_HEIGHT - 1 || stableMemory[currentShape.coordinates[i][0] + 1][currentShape.coordinates[i][1]])
                {
                    for (int j = 0; j < 4; j++)
                    {
                        stableMemory[currentShape.coordinates[j][0]][currentShape.coordinates[j][1]] = 1;
                    }
                    currentShape.active = false;
                    scanAndClearGrid();
                    break;
                }
            }
        }
    }
}

void genShape()
{
    if (!currentShape.active)
    {
        // Define possible shapes as relative coordinates
        const int shapes[7][4][2] = {
            {{0, 0}, {0, 1}, {1, 0}, {1, 1}},  // O shape
            {{0, 0}, {0, 1}, {0, 2}, {0, 3}},  // I shape
            {{0, 0}, {1, 0}, {1, 1}, {1, 2}},  // J shape
            {{0, 0}, {1, 0}, {0, 1}, {0, 2}},  // L shape
            {{0, 0}, {0, 1}, {1, 1}, {1, 2}},  // S shape
            {{0, 0}, {0, 1}, {1, 0}, {1, -1}}, // Z shape
            {{0, 0}, {0, 1}, {1, 0}, {0, -1}}  // T shape
        };

        // Randomly select a shape
        int randomIndex = random(0, 7);
        memcpy(currentShape.shape, shapes[randomIndex], sizeof(currentShape.shape));

        // Initialize starting position
        for (int i = 0; i < 4; i++)
        {
            currentShape.coordinates[i][0] = currentShape.shape[i][0] + 1; // Start near the top
            currentShape.coordinates[i][1] = currentShape.shape[i][1] + GRID_WIDTH / 2;
        }

        currentShape.active = true;
    }
}

// Clears a specific row in the stable memory
void clearRow(int row)
{
    memset(stableMemory[row], 0, sizeof(stableMemory[row]));
}

// Shifts all rows down starting from a specific row
void shiftRowsDown(int startRow)
{
    for (int i = startRow; i > 0; i--)
    {
        memcpy(stableMemory[i], stableMemory[i - 1], sizeof(stableMemory[i]));
    }
}

// Scans the grid for full rows, clears them, and updates the score
void scanAndClearGrid()
{
    for (int i = 0; i < 3; i++)
    { // Check for overflow in illegal zone
        for (int j = 0; j < GRID_WIDTH; j++)
        {
            if (stableMemory[i][j] == 1)
            {
                end = true; // Game over
                return;
            }
        }
    }

    int clearedRows = 0;
    for (int i = CHECKING_HEIGHT - 1; i >= 3; i--)
    {
        bool isFull = true;
        for (int j = 0; j < GRID_WIDTH; j++)
        {
            if (stableMemory[i][j] == 0)
            {
                isFull = false;
                break;
            }
        }

        if (isFull)
        {
            clearRow(i);
            clearedRows++;
        }
        else if (clearedRows > 0)
        {
            shiftRowsDown(i + clearedRows);
        }
    }

    // Update the score based on rows cleared
    if (clearedRows > 0)
    {
        score += pow(clearedRows, clearedRows);
        sendScore(score);
    }
}

// Handles a button press with the given action
void handleButtonPress(int buttonPin, void (*action)())
{
    if (digitalRead(buttonPin) == HIGH)
    {
        action();
    }
}

// Check for user inputs and trigger the appropriate actions
void checkInput()
{
    handleButtonPress(LEFT_PIN, moveLeft);
    handleButtonPress(RIGHT_PIN, moveRight);
    handleButtonPress(DOWN_PIN, moveDown);
    handleButtonPress(ROTATE_PIN, rotateShape);
}

// Updates the display memory and sends it to the LED matrices
void gatherAndDisplay()
{
    memcpy(displayMemory, stableMemory, sizeof(stableMemory));
    for (int i = 0; i < 4; i++)
    {
        displayMemory[currentShape.coordinates[i][0]][currentShape.coordinates[i][1]] = 1;
    }

    for (int i = 0; i < GRID_WIDTH; i++)
    {
        memcpy(topLM[i], &displayMemory[i + 2][0], sizeof(topLM[i]));
        memcpy(botLM[i], &displayMemory[i + GRID_WIDTH + 2][0], sizeof(botLM[i]));
    }

    LMtop.Symbol(topLM, 2); // Send to top LED matrix
    LMbot.Symbol(botLM, 2); // Send to bottom LED matrix
}

// Sends the score to the serial connection
void sendScore(int score)
{
    Serial8.write(lowByte(score));
    Serial8.write(highByte(score));
}

// Moves the current shape left
void moveLeft()
{
    alterShape(-1); // Implement alterShape for side movement
}

// Moves the current shape right
void moveRight()
{
    alterShape(1);
}

// Moves the current shape down
void moveDown()
{
    alterShape(0);
}

// Rotates the current shape
void rotateShape()
{
    alterShape(3);
}

// Displays the end animation
void showEndAnimation()
{
    for (int i = GRID_HEIGHT - 1; i >= 0; i--)
    {
        for (int j = 0; j < GRID_WIDTH; j++)
        {
            displayMemory[i][j] = 0;
        }
        gatherAndDisplay();
        delay(50); // Small delay for animation effect
    }
}

// Arduino setup function
void setup()
{
    Serial.begin(9600);
    Serial8.begin(9600);

    for (int i = 30; i < 34; i++)
    {
        pinMode(i, INPUT);
    }

    randomSeed(analogRead(24)); // Initialize random number generator
    sendScore(10000);           // Initialize the other board
}

// Arduino main loop function
void loop()
{
    if (digitalRead(RESET_PIN) == HIGH)
    {
        sendScore(10001); // Reset call
    }

    if (ended)
    {
        LMbot.Symbol("E"); // Show 'End' symbol on the bottom matrix
    }
    else if (!end)
    {
        genShape();   // Generate a new shape if needed
        checkInput(); // Process player inputs

        if (millis() - prev >= interval)
        {
            prev = millis();
            moveDown(); // Move the shape down automatically
        }

        gatherAndDisplay(); // Update display
    }
    else
    {
        showEndAnimation();
        ended = true;
    }
}

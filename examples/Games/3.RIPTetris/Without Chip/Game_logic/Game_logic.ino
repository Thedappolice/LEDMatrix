#include <math.h> // Math library for functions like pow()

// Pin Definitions for Controls
#define ROTATE_PIN 32
#define LEFT_OR_RIGHT_PIN 23
#define UP_OR_DOWN_PIN 22

// Grid Constants
#define GRID_WIDTH 8                      // Number of columns in the grid
#define GRID_HEIGHT (GRID_WIDTH * 2)      // Total grid height (16 rows for an 8x8 matrix)
#define CHECKING_HEIGHT (GRID_HEIGHT + 2) // Includes extra rows for overflow checking

// LED Matrix Library and Pin Configurations
#include <LEDMatrix.h>

// Pin configurations for the top LED matrix
int posPintop[] = {33, 38, 41, 36, 19, 13, 18, 15};
int negPintop[] = {37, 17, 16, 34, 14, 35, 39, 40};

// Pin configurations for the bottom LED matrix
int posPinbot[] = {9, 4, 28, 6, 10, 27, 11, 25};
int negPinbot[] = {5, 12, 24, 8, 26, 7, 3, 2};

// Create instances for LED matrices
LEDMatrix<GRID_WIDTH, GRID_WIDTH> LMtop(posPintop, negPintop);
LEDMatrix<GRID_WIDTH, GRID_WIDTH> LMbot(posPinbot, negPinbot);

// Game Data Structures

// Represents a Tetrimino (Tetris shape)
typedef struct
{
    int shape[4][2];       // Relative positions of blocks in the shape
    int coordinates[4][2]; // Absolute positions of blocks on the grid
    bool active;           // Whether the shape is currently active
} Tetrimino;

// Global Variables

Tetrimino currentShape = {0}; // Current active Tetrimino

// Memory structure for the game grid
struct GridMemory
{
    int stable[CHECKING_HEIGHT][GRID_WIDTH];  // Stable grid (occupied blocks)
    int display[CHECKING_HEIGHT][GRID_WIDTH]; // Display grid (active shapes + stable grid)
    int top[GRID_WIDTH][GRID_WIDTH];          // Top LED matrix data
    int bottom[GRID_WIDTH][GRID_WIDTH];       // Bottom LED matrix data

    // Clears the stable grid
    void clearStable()
    {
        memset(stable, 0, sizeof(stable));
    }

    // Clears the display grid
    void clearDisplay()
    {
        memset(display, 0, sizeof(display));
    }
};

// Create an instance of GridMemory
GridMemory grid;

// Timing Variables
unsigned long lastInputTime = 0;   // Last time input was processed
unsigned long inputInterval = 100; // Interval between inputs (ms)
unsigned long prev = 0;            // Previous time for game interval
unsigned long interval = 500;      // Time between automatic block drops (ms)

// Track the previous state of the rotate button
bool previousRotateState = LOW;

// Game State Variables
int score = 0;      // Current score
bool end = false;   // Whether the game is over
bool ended = false; // Whether the end animation has been shown

/// --- Function Definitions ---

/**
 * Displays a predefined symbol on the LED matrix.
 * @param LM - Reference to the LEDMatrix object to display on.
 * @param input - Character representing the symbol ('1', '2', '3', or 'E').
 * @param duration - Optional duration (in milliseconds) for displaying the symbol. Default is 0 (persistent display).
 */
void Char(LEDMatrix<8, 8> &LM, char input, unsigned long duration = 0)
{
    // Predefined symbols
    int symbols[4][8][8] = {
        {// Symbol '1'
         {0, 0, 0, 1, 1, 0, 0, 0},
         {0, 0, 1, 1, 1, 0, 0, 0},
         {0, 1, 1, 1, 1, 0, 0, 0},
         {0, 0, 0, 1, 1, 0, 0, 0},
         {0, 0, 0, 1, 1, 0, 0, 0},
         {0, 0, 0, 1, 1, 0, 0, 0},
         {0, 1, 1, 1, 1, 1, 1, 0},
         {0, 1, 1, 1, 1, 1, 1, 0}},
        {// Symbol '2'
         {0, 0, 1, 1, 1, 1, 0, 0},
         {0, 1, 1, 1, 1, 1, 1, 0},
         {0, 1, 1, 0, 0, 1, 1, 0},
         {0, 0, 0, 0, 1, 1, 1, 0},
         {0, 0, 0, 1, 1, 1, 0, 0},
         {0, 0, 1, 1, 1, 0, 0, 0},
         {0, 1, 1, 1, 1, 1, 1, 0},
         {0, 1, 1, 1, 1, 1, 1, 0}},
        {// Symbol '3'
         {0, 0, 1, 1, 1, 1, 0, 0},
         {0, 1, 1, 1, 1, 1, 1, 0},
         {0, 0, 0, 0, 0, 1, 1, 0},
         {0, 0, 1, 1, 1, 1, 0, 0},
         {0, 0, 1, 1, 1, 1, 0, 0},
         {0, 0, 0, 0, 0, 1, 1, 0},
         {0, 1, 1, 1, 1, 1, 1, 0},
         {0, 0, 1, 1, 1, 1, 0, 0}},
        {// Symbol 'E' (End)
         {0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0},
         {1, 1, 1, 0, 0, 0, 0, 1},
         {1, 0, 0, 0, 0, 0, 0, 1},
         {1, 1, 0, 0, 1, 0, 0, 1},
         {1, 0, 0, 1, 0, 1, 1, 0},
         {1, 1, 1, 1, 0, 1, 0, 1},
         {0, 0, 0, 0, 0, 0, 0, 0}}};

    // Map the input character to the symbol index
    int symbolIndex = -1;
    switch (input)
    {
    case '1':
        symbolIndex = 0;
        break;
    case '2':
        symbolIndex = 1;
        break;
    case '3':
        symbolIndex = 2;
        break;
    case 'E':
        symbolIndex = 3;
        break;
    default:
        break;
    }

    // Display the symbol on the LED matrix
    if (symbolIndex != -1)
    {
        (duration > 0) ? LM.Symbol(symbols[symbolIndex], duration) : LM.Symbol(symbols[symbolIndex]);
    }
}

/**
 * Updates the position or rotation of the current shape.
 * @param mode - Movement or rotation mode:
 *               -1: Left, 1: Right, 0: Down, 3: Rotate
 */
void alterShape(int mode)
{
    int newCoordinates[4][2]; // Temporary coordinates for validation

    for (int i = 0; i < 4; i++)
    {
        newCoordinates[i][0] = currentShape.coordinates[i][0];
        newCoordinates[i][1] = currentShape.coordinates[i][1];

        if (mode == -1) // Move left
            newCoordinates[i][1] -= 1;
        else if (mode == 1) // Move right
            newCoordinates[i][1] += 1;
        else if (mode == 0) // Move down
            newCoordinates[i][0] += 1;
        else if (mode == 3) // Rotate
        {
            int pivotX = currentShape.coordinates[0][1]; // Use first block as pivot
            int pivotY = currentShape.coordinates[0][0];

            for (int j = 0; j < 4; j++)
            {
                int relX = currentShape.coordinates[j][1] - pivotX;
                int relY = currentShape.coordinates[j][0] - pivotY;

                newCoordinates[j][0] = pivotY - relX; // Rotate 90° clockwise
                newCoordinates[j][1] = pivotX + relY;
            }
        }
    }

    // Validate new position
    bool validMove = true;
    for (int i = 0; i < 4; i++)
    {
        int y = newCoordinates[i][0];
        int x = newCoordinates[i][1];

        if (y >= CHECKING_HEIGHT || x < 0 || x >= GRID_WIDTH || grid.stable[y][x])
        {
            validMove = false;
            break;
        }
    }

    if (validMove)
    {
        memcpy(currentShape.coordinates, newCoordinates, sizeof(newCoordinates));
    }
}

/**
 * Generates a new random Tetrimino at the top of the grid.
 */
void genShape()
{
    if (!currentShape.active)
    {
        const int shapes[7][4][2] = {
            {{0, 0}, {0, 1}, {1, 0}, {1, 1}},  // O shape
            {{0, 0}, {0, 1}, {0, 2}, {0, 3}},  // I shape
            {{0, 0}, {1, 0}, {1, 1}, {1, 2}},  // J shape
            {{0, 0}, {1, 0}, {0, 1}, {0, 2}},  // L shape
            {{0, 0}, {0, 1}, {1, 1}, {1, 2}},  // S shape
            {{0, 0}, {0, 1}, {1, 0}, {1, -1}}, // Z shape
            {{0, 0}, {0, 1}, {1, 0}, {0, -1}}  // T shape
        };

        // Randomly select a shape and initialize it
        int randomIndex = random(0, 7);
        memcpy(currentShape.shape, shapes[randomIndex], sizeof(currentShape.shape));

        for (int i = 0; i < 4; i++)
        {
            currentShape.coordinates[i][0] = currentShape.shape[i][0] + 1;
            currentShape.coordinates[i][1] = currentShape.shape[i][1] + GRID_WIDTH / 2;
        }

        currentShape.active = true;
    }
}

/**
 * Clears a specific row in the stable memory.
 * @param row - The row to be cleared.
 */
void clearRow(int row)
{
    memset(grid.stable[row], 0, sizeof(grid.stable[row]));
}

/**
 * Shifts all rows down starting from a specific row.
 * @param startRow - The row to start shifting down from.
 */
void shiftRowsDown(int startRow)
{
    for (int i = startRow; i > 0; i--)
    {
        memcpy(grid.stable[i], grid.stable[i - 1], sizeof(grid.stable[i]));
    }
}

/**
 * Scans the grid for full rows, clears them, and updates the score.
 */
void scanAndClearGrid()
{
    int clearedRows = 0;

    // Check and clear full rows from bottom to top
    for (int i = CHECKING_HEIGHT - 1; i >= 3; i--)
    {
        bool isFull = true;
        for (int j = 0; j < GRID_WIDTH; j++)
        {
            if (grid.stable[i][j] == 0)
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

    // Update score based on the number of cleared rows
    const int scoreTable[] = {0, 40, 100, 300, 1200}; // Standard Tetris scoring
    if (clearedRows > 0)
    {
        score += scoreTable[clearedRows];
        if (score > 9999)
            score = 9999; // Cap score at 9999

        // Increase game speed every 10 lines cleared
        static int totalCleared = 0;
        totalCleared += clearedRows;
        if (totalCleared >= 10)
        {
            totalCleared = 0;
            interval = max(100, interval - 50); // Decrease interval (increase speed)
        }

        sendScore(score);
    }
}

/**
 * Checks for user inputs and triggers the appropriate actions.
 */
void checkInput()
{
    // Read the current state of the rotation button
    bool currentRotateState = digitalRead(ROTATE_PIN);

    // Detect rising edge: from LOW to HIGH
    if (currentRotateState == HIGH && previousRotateState == LOW)
    {
        alterShape(3); // Rotate once
    }

    // Update the previous state
    previousRotateState = currentRotateState;

    // // Other button inputs (no changes needed)
    int Xvalue = analogRead(LEFT_OR_RIGHT_PIN);
    int Yvalue = analogRead(UP_OR_DOWN_PIN);

    if (Xvalue > 900)
    {
        alterShape(1); // Move Left
    }
    if (Xvalue < 100)
    {
        alterShape(-1); // Move Right
    }
    if (Yvalue < 100)
    {
        alterShape(0); // Move Down
    }
}

/**
 * Updates the display memory and sends it to the LED matrices.
 */
void gatherAndDisplay()
{
    if (!end)
    { // Copy the stable grid into the display grid
        memcpy(grid.display, grid.stable, sizeof(grid.stable));

        // Add the active shape to the display grid
        for (int i = 0; i < 4; i++)
        {
            grid.display[currentShape.coordinates[i][0]][currentShape.coordinates[i][1]] = 1;
        }
    }

    // Update the top and bottom matrix displays
    for (int i = 0; i < GRID_WIDTH; i++)
    {
        memcpy(grid.top[i], &grid.display[i + 2][0], sizeof(grid.top[i]));                    // Top matrix
        memcpy(grid.bottom[i], &grid.display[i + GRID_WIDTH + 2][0], sizeof(grid.bottom[i])); // Bottom matrix
    }

    // Send updated data to the LED matrices
    LMtop.Symbol(grid.top, 2);
    LMbot.Symbol(grid.bottom, 2);
}

/**
 * Displays the "end game" animation.
 */
void showEndAnimation()
{
    for (int i = CHECKING_HEIGHT; i > 0; i--)
    {
        for (int j = 0; j < GRID_WIDTH; j++)
        {
            grid.display[i][j] = 0; // Clear rows one by one
        }
        gatherAndDisplay();
        delay(50); // Small delay for animation effect
    }
}

/**
 * Sends the score to another board or device.
 * @param score - The current score to be sent.
 */
void sendScore(int score)
{
    Serial5.write(lowByte(score));  // Send the lower byte
    Serial5.write(highByte(score)); // Send the higher byte
}

/// --- Arduino Setup and Main Loop ---

/**
 * Arduino setup function, initializes pins and the game.
 */
void setup()
{
    Serial5.begin(9600); // Initialize Serial7 for score transmission

    // Initialize control pins as input
    pinMode(ROTATE_PIN, INPUT);

    randomSeed(analogRead(21)); // Seed the random number generator
    sendScore(10000);           // Signal game start to another board
}

/**
 * Arduino main loop function, runs the game logic continuously.
 */
void loop()
{
    static unsigned long lastDropTime = 0;
    static unsigned long lastMoveTime = 0;
    unsigned long currentTime = millis();

    if (!end)
    {
        checkInput();

        // Handle automatic downward movement every "interval" milliseconds
        if (currentTime - lastDropTime >= interval)
        {
            lastDropTime = currentTime;
            alterShape(0); // Move shape down automatically
        }

        // Add input delay (100ms) for smoother movement
        if (currentTime - lastMoveTime >= inputInterval)
        {
            lastMoveTime = currentTime;
            checkInput(); // Read inputs with a small delay
        }

        gatherAndDisplay();
    }
    else
    {
        showEndAnimation();
        ended = true;
    }
}

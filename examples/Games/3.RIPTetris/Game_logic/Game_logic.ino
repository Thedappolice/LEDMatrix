
#include <LEDMatrix.h>

// ============================================================================
// HARDWARE CONFIGURATION
// ============================================================================

enum Action
{
    LOCK = 0,
    SHIFT = 1,
    DROP = 2,
    ROTATE = 3,
    UPDATE = 4
};

// Control Buttons
#define ROTATE_PIN 32
// Joystick Axis Pins
#define JOYSTICK_X_PIN 23 // Left/Right
#define JOYSTICK_Y_PIN 22 // Down

// LED Matrix 1 (Top) - Rows 0-7
int posPintop[] = {19, 18, 17, 16, 15, 14, 13, 41};
int negPintop[] = {40, 39, 38, 37, 36, 35, 34, 33};

// LED Matrix 2 (Bottom) - Rows 8-15
int posPinbot[] = {2, 3, 4, 5, 6, 7, 8, 9};
int negPinbot[] = {10, 11, 12, 24, 25, 26, 27, 28};

// CORRECT instantiation for your library
LEDMatrix<8, 8> LMtop(posPintop, negPintop);
LEDMatrix<8, 8> LMbot(posPinbot, negPinbot);

// ============================================================================
// GAME GRID CONSTANTS
// ============================================================================

const int GRID_W = 8;
const int GRID_H = 16;
const int BUFFER_H = GRID_H + 2;

// ============================================================================
// MEMORY STRUCTURES
// ============================================================================

int locked[BUFFER_H][GRID_W] = {{0}};
int display[BUFFER_H][GRID_W] = {{0}};

// LED Buffers - MUST be [8][8] for your library's Symbol() method
int topBuf[8][8] = {{0}};
int botBuf[8][8] = {{0}};

// ============================================================================
// TETROMINO STATE
// ============================================================================

int currentPiece[4][2] = {{0}};
int absolutePos[4][2] = {{0}};

bool hasPiece = false;
int shiftDir = 0;

// ============================================================================
// WALL KICK DATA
// ============================================================================

const int WALL_KICK_TESTS[][2] = {
    {0, 0},   // Normal rotation
    {0, -1},  // Left 1
    {0, 1},   // Right 1
    {-1, 0},  // Up 1
    {0, -2},  // Left 2
    {0, 2},   // Right 2
    {-1, -1}, // Up-left
    {-1, 1}   // Up-right
};

const int NUM_KICK_TESTS = 8;

// ============================================================================
// GAME STATE
// ============================================================================

bool gameOver = false;
bool enableScoring = true;
int score = 0;

// Timing
unsigned long fallInterval = 500;
unsigned long lastFallTime = 0;
unsigned long lastInputTime = 0;
unsigned long inputInterval = 100;
unsigned long lastRotateTime = 0;
unsigned long rotateInterval = 300;

// Display refresh timing
unsigned long lastDisplayTime = 0;
unsigned long displayInterval = 2; // Refresh display every 2ms

int prevButtonStates = 0;

// ============================================================================
// TETROMINO DEFINITIONS
// ============================================================================

int PIECE_J[4][2] = {{0, 3}, {1, 1}, {0, 1}, {0, -1}};
int PIECE_L[4][2] = {{0, 3}, {1, -1}, {0, 1}, {0, -1}};
int PIECE_S[4][2] = {{0, 3}, {0, 1}, {1, 0}, {1, -1}};
int PIECE_Z[4][2] = {{0, 3}, {0, -1}, {1, 0}, {1, 1}};
int PIECE_T[4][2] = {{0, 3}, {0, 1}, {1, 0}, {0, -1}};
int PIECE_O[4][2] = {{0, 3}, {1, 0}, {0, 1}, {1, 1}};
int PIECE_I[4][2] = {{0, 3}, {0, 1}, {0, -1}, {0, -2}};

int *pieceTemplates[7] = {
    (int *)PIECE_J, (int *)PIECE_L, (int *)PIECE_S, (int *)PIECE_Z,
    (int *)PIECE_T, (int *)PIECE_O, (int *)PIECE_I};

// ============================================================================
// HELPER FUNCTIONS
// ============================================================================

void calculatePositions()
{
    absolutePos[0][0] = currentPiece[0][0];
    absolutePos[0][1] = currentPiece[0][1];

    for (int i = 1; i < 4; i++)
    {
        absolutePos[i][0] = currentPiece[0][0] + currentPiece[i][0];
        absolutePos[i][1] = currentPiece[0][1] + currentPiece[i][1];
    }
}

bool isPositionValid()
{
    calculatePositions();

    for (int i = 0; i < 4; i++)
    {
        int row = absolutePos[i][0];
        int col = absolutePos[i][1];

        if (col < 0 || col >= GRID_W || row < 0 || row >= BUFFER_H)
        {
            return false;
        }

        if (locked[row][col] == 1)
        {
            return false;
        }
    }

    return true;
}

// ============================================================================
// PIECE OPERATIONS
// ============================================================================

void lockPiece()
{
    calculatePositions();
    for (int i = 0; i < 4; i++)
    {
        locked[absolutePos[i][0]][absolutePos[i][1]] = 1;
    }
}

void shiftPiece()
{
    int originalCol = currentPiece[0][1];
    currentPiece[0][1] += shiftDir;

    if (!isPositionValid())
    {
        currentPiece[0][1] = originalCol;
    }
}

void dropPiece()
{
    int originalRow = currentPiece[0][0];
    currentPiece[0][0]++;

    if (!isPositionValid())
    {
        currentPiece[0][0] = originalRow;
        lockPiece();
        hasPiece = false;
        clearCompletedLines();
    }
}

void rotatePiece()
{
    int savedOffsets[3][2];
    int savedAnchorRow = currentPiece[0][0];
    int savedAnchorCol = currentPiece[0][1];

    for (int i = 0; i < 3; i++)
    {
        savedOffsets[i][0] = currentPiece[i + 1][0];
        savedOffsets[i][1] = currentPiece[i + 1][1];
    }

    // Rotate: (row, col) → (col, -row)
    for (int i = 1; i < 4; i++)
    {
        int temp = currentPiece[i][0];
        currentPiece[i][0] = currentPiece[i][1];
        currentPiece[i][1] = -temp;
    }

    // Try wall kicks
    bool rotationSucceeded = false;

    for (int test = 0; test < NUM_KICK_TESTS; test++)
    {
        currentPiece[0][0] = savedAnchorRow + WALL_KICK_TESTS[test][0];
        currentPiece[0][1] = savedAnchorCol + WALL_KICK_TESTS[test][1];

        if (isPositionValid())
        {
            rotationSucceeded = true;
            break;
        }
    }

    // Revert if all kicks failed
    if (!rotationSucceeded)
    {
        currentPiece[0][0] = savedAnchorRow;
        currentPiece[0][1] = savedAnchorCol;

        for (int i = 1; i < 4; i++)
        {
            currentPiece[i][0] = savedOffsets[i - 1][0];
            currentPiece[i][1] = savedOffsets[i - 1][1];
        }
    }
}

void handlePiece(Action action)
{
    switch (action)
    {
    case LOCK:
        lockPiece();
        break;
    case SHIFT:
        shiftPiece();
        break;
    case DROP:
        dropPiece();
        break;
    case ROTATE:
        rotatePiece();
        break;
    case UPDATE:
        calculatePositions();
        break;
    }
}

// ============================================================================
// PIECE GENERATION
// ============================================================================

void spawnPiece()
{
    if (!hasPiece)
    {
        int shapeIndex = random(0, 7);
        memcpy(currentPiece, pieceTemplates[shapeIndex], 8 * sizeof(int));
        hasPiece = true;
        handlePiece(UPDATE);
    }
}

// ============================================================================
// INPUT HANDLING
// ============================================================================

int readButtons()
{
    int state = 0;

    int xVal = analogRead(JOYSTICK_X_PIN);
    int yVal = analogRead(JOYSTICK_Y_PIN);

    if (xVal > 900)
    {
        state |= (1 << 0);
    }

    if (xVal < 388)
    {
        state |= (1 << 1);
    }

    if (yVal > 921)
    {
        state |= (1 << 2);
    }

    if (digitalRead(ROTATE_PIN) == HIGH)
    {
        state |= (1 << 3);
    }

    return state;
}

void processInput()
{
    int current = readButtons();
    int pressed = current & ~prevButtonStates;

    if (pressed & (1 << 0))
    {
        shiftDir = -1;
        handlePiece(SHIFT);
    }

    if (pressed & (1 << 1))
    {
        shiftDir = 1;
        handlePiece(SHIFT);
    }

    if (pressed & (1 << 2))
    {
        handlePiece(DROP);
    }

    if (pressed & (1 << 3))
    {
        if (millis() - lastRotateTime >= rotateInterval)
        {
            handlePiece(ROTATE);
            lastRotateTime = millis();
        }
    }

    prevButtonStates = current;
}

// ============================================================================
// LINE CLEARING & SCORING
// ============================================================================

// void clearCompletedLines();  // Forward declaration

// ============================================================================
// DISPLAY RENDERING
// ============================================================================

/**
 * Update display buffers (non-blocking)
 * Your library's Symbol() handles the actual multiplexing
 */
void updateDisplay()
{
    // Composite locked + active piece
    memcpy(display, locked, sizeof(locked));
    calculatePositions();

    for (int i = 0; i < 4; i++)
    {
        display[absolutePos[i][0]][absolutePos[i][1]] = 1;
    }

    // Split into 8x8 buffers for each matrix
    // Top matrix: rows 2-9 of game grid
    // Bottom matrix: rows 10-17 of game grid
    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            topBuf[row][col] = display[row + 2][col];
            botBuf[row][col] = display[row + 10][col];
        }
    }
}

/**
 * Refresh LEDs using your library's Symbol() method
 * CRITICAL: Use minimal showTime to avoid blocking!
 */
void refreshLEDs()
{
    if (!gameOver)
    {
        // Use 2ms showTime - your library will multiplex during this time
        LMtop.Symbol(topBuf, 2);
        LMbot.Symbol(botBuf, 2);
    }
}

// ============================================================================
// LINE CLEARING IMPLEMENTATION
// ============================================================================

void clearCompletedLines()
{
    // Game Over Check
    for (int row = 0; row < 2; row++)
    {
        for (int col = 0; col < GRID_W; col++)
        {
            if (locked[row][col] == 1)
            {
                gameOver = true;
                return;
            }
        }
    }

    // Find full rows
    int fullRows[4] = {-1, -1, -1, -1};
    int fullCount = 0;

    for (int row = 2; row < BUFFER_H; row++)
    {
        bool isFull = true;

        for (int col = 0; col < GRID_W; col++)
        {
            if (locked[row][col] == 0)
            {
                isFull = false;
                break;
            }
        }

        if (isFull && fullCount < 4)
        {
            fullRows[fullCount++] = row;
        }
    }

    // Clear and collapse
    if (fullCount > 0)
    {
        for (int i = 0; i < fullCount; i++)
        {
            for (int col = 0; col < GRID_W; col++)
            {
                locked[fullRows[i]][col] = 0;
            }
        }

        int topRow = fullRows[0];
        for (int row = topRow; row > 0; row--)
        {
            for (int col = 0; col < GRID_W; col++)
            {
                locked[row][col] = locked[row - 1][col];
            }
        }

        if (enableScoring)
        {
            int points = 1;
            for (int i = 0; i < fullCount; i++)
            {
                points *= fullCount;
            }
            score += points;
            sendScore(score);
        }
    }
}

// ============================================================================
// SERIAL COMMUNICATION
// ============================================================================

void sendScore(int value)
{
    Serial5.write(lowByte(value));
    Serial5.write(highByte(value));
}

// ============================================================================
// GAME LIFECYCLE
// ============================================================================

void setup()
{
    randomSeed(analogRead(29));

    Serial.begin(9600);
    Serial5.begin(9600);

    pinMode(ROTATE_PIN, INPUT);

    // Give Nano time to finish booting, then signal game start
    delay(3000);
    sendScore(10000); // 10000 = "game start" sentinel
}

void loop()
{
    if (gameOver)
    {
        sendScore(10001);
        return;
    }

    spawnPiece();

    // Input handling
    if (millis() - lastInputTime >= inputInterval)
    {
        lastInputTime = millis();
        processInput();
    }

    // Gravity
    if (millis() - lastFallTime >= fallInterval)
    {
        lastFallTime = millis();
        handlePiece(DROP);
    }

    // Update and refresh display
    updateDisplay();
    refreshLEDs(); // This calls Symbol() which blocks for 2ms
    // debugDisplay();
}

void debugDisplay()
{
    Serial.println("--- TETRIS GRID ---");
    // We iterate from row 2 to 17 because your game uses
    // rows 0-1 for spawn/buffer and rows 2-17 for the actual LED matrices
    for (int row = 2; row < BUFFER_H; row++)
    {
        Serial.print(row < 10 ? " " : ""); // Align row numbers
        Serial.print(row);
        Serial.print(" | ");

        for (int col = 0; col < GRID_W; col++)
        {
            if (display[row][col] == 1)
            {
                Serial.print("[x]"); // Block present
            }
            else
            {
                Serial.print("[ ]"); // Empty space
            }
        }
        Serial.println(" |");
    }
    Serial.println("-------------------");
}

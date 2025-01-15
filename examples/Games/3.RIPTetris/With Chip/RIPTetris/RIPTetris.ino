// this program uses Arduino Nano

#include <math.h>

#include <Dis7seg.h>
int score = 0;

#define reset_pin 36

#define ROTATE_PIN 31
#define LEFT_PIN 32
#define RIGHT_PIN 33
#define DOWN_PIN 30
#include <LEDMatrix.h>

// initialize matrix!!

// Grid definition
const int width = 8;
const int height = 2 * width;

// height + 2 for checking purposes
const int checkingHeight = height + 2;

// memories for memory and display
int displayMemory[checkingHeight][width] = {{0}};
int stableMemory[checkingHeight][width] = {{0}};

// arrays for top and bottom led matrix
int topLM[width][width] = {{0}};
int botLM[width][width] = {{0}};

// the shape in use in (x, y)
int currentShape[4][2] = {{0}};
int shapeCoordinates[4][2]; // in (y, x)

// determinants
bool gotShape = false;
int command = -2;
bool end = false;
bool ended = false;

// timing the game
unsigned long interval = 500; // downwards interval
unsigned long prev = 0;

int lastButtonState = 0;

unsigned long lastRotateTime = 0;   // Timestamp for the last rotation
unsigned long rotateInterval = 300; // Interval for rotation (in milliseconds)

unsigned long lastInputTime = 0;
unsigned long inputInterval = 100; // Adjust this based on how frequently you want to check input

void ShowSymbol(LEDMatrix &LM, char input, unsigned long duration = 0)
{
    static const int End[8][8] = {
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 0, 0, 1, 0, 0, 1},
        {1, 0, 0, 1, 0, 1, 1, 0},
        {1, 1, 1, 1, 0, 1, 0, 1},
        {0, 0, 0, 0, 0, 0, 0, 0}};
    static const int N1[8][8] = {
        {0, 0, 0, 1, 1, 0, 0, 0},
        {0, 0, 1, 1, 1, 0, 0, 0},
        {0, 1, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 1, 1, 0, 0, 0},
        {0, 0, 0, 1, 1, 0, 0, 0},
        {0, 0, 0, 1, 1, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 0},
        {0, 1, 1, 1, 1, 1, 1, 0}};
    static const int N2[8][8] = {
        {0, 0, 1, 1, 1, 1, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 0},
        {0, 1, 1, 0, 0, 1, 1, 0},
        {0, 0, 0, 0, 1, 1, 1, 0},
        {0, 0, 0, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 0},
        {0, 1, 1, 1, 1, 1, 1, 0}};
    static const int N3[8][8] = {
        {0, 0, 1, 1, 1, 1, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 1, 1, 0},
        {0, 0, 1, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 1, 0, 0},
        {0, 0, 0, 0, 0, 1, 1, 0},
        {0, 1, 1, 1, 1, 1, 1, 0},
        {0, 0, 1, 1, 1, 1, 0, 0}};

    const int(*Matrix)[8] = nullptr;
    switch (input)
    {
    case '1':
        Matrix = N1;
        break;
    case '2':
        Matrix = N2;
        break;
    case '3':
        Matrix = N3;
        break;
    case 'E':
        Matrix = End;
        break;
    default:
        return; // Exit if input is invalid
    }

    (duration > 0) ? LM.Symbol(Matrix, duration) : LM.Symbol(Matrix);
};


void genShape()
{
    if (!gotShape)
    {
        static const int J[4][2] = {{0, 3}, {0, 1}, {180, 1}, {135, 1}};
        static const int L[4][2] = {{0, 3}, {0, 1}, {180, 1}, {45, 1}};
        static const int S[4][2] = {{0, 3}, {0, 1}, {90, 1}, {135, 1}};
        static const int Z[4][2] = {{0, 3}, {180, 1}, {90, 1}, {45, 1}};
        static const int T[4][2] = {{0, 3}, {0, 1}, {90, 1}, {180, 1}};
        static const int O[4][2] = {{0, 3}, {0, 1}, {45, 1}, {90, 1}};
        static const int I[4][2] = {{0, 3}, {0, 1}, {180, 1}, {180, 2}};

        static const int(*shapes[])[4][2] = {&J, &L, &S, &Z, &T, &O, &I};

        memcpy(currentShape, *shapes[random(0, 7)], sizeof(currentShape));
        gotShape = true;
        alterShape();
    }
}

void alterShape(int req = -1)
{
    static const int sinTable[4] = {0, 1, 0, -1}; // 0°, 90°, 180°, 270°
    static const int cosTable[4] = {1, 0, -1, 0}; 

    auto isInBounds = [](int x, int y) {
        return (x >= 0 && x < checkingHeight && y >= 0 && y < width);
    };

    switch (req)
    {
    case 0: // Record shape
        for (int i = 0; i < 4; i++)
        {
            stableMemory[shapeCoordinates[i][0]][shapeCoordinates[i][1]] = 1;
        }
        break;

    case 1: // Shift
        for (int i = 0; i < 4; i++)
        {
            int newX = shapeCoordinates[i][1] + command;
            if (!isInBounds(shapeCoordinates[i][0], newX) || stableMemory[shapeCoordinates[i][0]][newX])
                return; // Exit if not shiftable
        }
        currentShape[0][1] += command;
        break;

    case 2: // Move downwards
        for (int i = 0; i < 4; i++)
        {
            int newY = shapeCoordinates[i][0] + 1;
            if (!isInBounds(newY, shapeCoordinates[i][1]) || stableMemory[newY][shapeCoordinates[i][1]])
            {
                alterShape(0);
                gotShape = false;
                scanAndClearGrid();
                return;
            }
        }
        currentShape[0][0]++;
        break;

    case 3: // Rotate
        int backup[4][2];
        memcpy(backup, currentShape, sizeof(currentShape));

        for (int i = 1; i < 4; i++)
        {
            int relY = shapeCoordinates[i][0] - shapeCoordinates[0][0];
            int relX = shapeCoordinates[i][1] - shapeCoordinates[0][1];

            currentShape[i][0] = shapeCoordinates[0][0] - relX; // y' = -x
            currentShape[i][1] = shapeCoordinates[0][1] + relY; // x' = y
        }

        alterShape();
        for (int i = 0; i < 4; i++)
        {
            if (!isInBounds(shapeCoordinates[i][0], shapeCoordinates[i][1]) || stableMemory[shapeCoordinates[i][0]][shapeCoordinates[i][1]])
            {
                memcpy(currentShape, backup, sizeof(currentShape));
                break;
            }
        }
        break;
    }
}

// Function to get the combined state of all buttons
int getButtonState()
{
    int state = 0;
    if (digitalRead(LEFT_PIN) == HIGH)
        state |= (1 << 0); // Set bit 0 for LEFT_PIN
    if (digitalRead(RIGHT_PIN) == HIGH)
        state |= (1 << 1); // Set bit 1 for RIGHT_PIN
    if (digitalRead(DOWN_PIN) == HIGH)
        state |= (1 << 2); // Set bit 2 for DOWN_PIN
    if (digitalRead(ROTATE_PIN) == HIGH)
        state |= (1 << 3); // Set bit 3 for ROTATE_PIN
    return state;
};

void checkInput()
{
    int currentButtonState = getButtonState(); // Get current button states

    // Check for any change in the button state
    if (currentButtonState != lastButtonState)
    {
        // Determine which button has been pressed
        if (currentButtonState & (1 << 0) && !(lastButtonState & (1 << 0)))
        {                  // LEFT pressed
            command = -1;  // Move left
            alterShape(1); // Perform the action
        }
        else if (currentButtonState & (1 << 1) && !(lastButtonState & (1 << 1)))
        {                // RIGHT pressed
            command = 1; // Move right
            alterShape(1);
        }
        else if (currentButtonState & (1 << 2) && !(lastButtonState & (1 << 2)))
        {                // DOWN pressed
            command = 0; // Move down
            alterShape(2);
        }
        else if (currentButtonState & (1 << 3) && !(lastButtonState & (1 << 3)))
        { // ROTATE pressed
            // Check if enough time has passed since the last rotation
            if (millis() - lastRotateTime >= rotateInterval)
            {
                command = 2; // Rotate
                alterShape(3);
                lastRotateTime = millis(); // Update the last rotation time
            }
        }
    }

    lastButtonState = currentButtonState; // Update last state
};

void scanAndClearGrid()
{
    bool rowClear[checkingHeight] = {false};

    // Identify rows to clear
    for (int y = 0; y < checkingHeight; y++)
    {
        rowClear[y] = true;
        for (int x = 0; x < width; x++)
        {
            if (!stableMemory[y][x])
            {
                rowClear[y] = false;
                break;
            }
        }
    }

    // Shift rows down
    for (int y = checkingHeight - 1; y >= 0; y--)
    {
        if (rowClear[y])
        {
            for (int shiftY = y; shiftY > 0; shiftY--)
            {
                memcpy(stableMemory[shiftY], stableMemory[shiftY - 1], sizeof(stableMemory[shiftY]));
            }
            memset(stableMemory[0], 0, sizeof(stableMemory[0])); // Clear top row
            y++; // Recheck shifted row
        }
    }
}

void gatherThenShowDisplay(bool skip = false)
{
    if (!skip)
    {
        memcpy(displayMemory, stableMemory, sizeof(stableMemory));
        for (int i = 0; i < 4; i++)
        {
            displayMemory[shapeCoordinates[i][0]][shapeCoordinates[i][1]] = 1;
        }
    }

    // Map `displayMemory` to top and bottom LED matrix arrays
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (i < width) // Top LED matrix
                topLM[i][j] = displayMemory[i][j];
            else // Bottom LED matrix
                botLM[i - width][j] = displayMemory[i][j];
        }
    }

    // Update matrices
    LEDMatrix topMatrix, botMatrix; // Ensure these are properly initialized
    topMatrix.Show(topLM);
    botMatrix.Show(botLM);
}


void endAnimation() // change this
{
    for (int i = height - 1; i > -1; i--) // delete and show the entire display
    {
        for (int j = width - 1; j > -1; j--) // Correct loop counter
        {
            displayMemory[i + 2][j] = 0;
            gatherThenShowDisplay(true);
        }
    }
};

void showScore()
{
    for (int i = 3; i >= 0; i--)
    {
        scoreNum[i] = score % 10; // Get the last digit
        score /= 10;              // Remove the last digit from input
    }

    // Handle leading zeros by replacing them with -1 to leave the digit blank
    for (int i = 0; i < 3; i++)
    {
        if (scoreNum[i] == 0 && (i == 0 || scoreNum[i - 1] == -1))
        {
            scoreNum[i] = -1; // Set leading zeroes to -1 (blank)
        }
    }
    // Update the 7-segment display with the new score
    dis.scan(scoreNum); // Refresh the display with updated score
};

void setup()
{
    randomSeed(analogRead(24)); // random generator

    for (int i = 30; i < 34; i++)
    {
        pinMode(i, INPUT);
    }

    // change these
    ShowSymbol(LMtop, '3', 500);
    ShowSymbol(LMtop, '2', 500);
    ShowSymbol(LMtop, '1', 500);
}

void loop()
{

    if (ended)
    {
        ShowSymbol(LMbot, 'E');
    }
    else
    {
        if (!end)
        {
            // Generate shape if none
            genShape();

            // Check inputs with edge detection (instant response after release)
            checkInput();

            // Main game logic (running on its own timing)
            if (millis() - prev >= interval)
            {
                prev = millis();
                alterShape(2); // Move down by default
            }

            // Update display
            gatherThenShowDisplay();

            showScore();
        }
        else
        {
            endAnimation();
            ended = true;
        }
    }
}
bool scoring = true;
int score = 0;

#define ROTATE_PIN 31
#define LEFT_PIN 32
#define RIGHT_PIN 33
#define DOWN_PIN 30
#include <LEDMatrix.h>

// Pin configurations of 1st Led matrix
int posPintop[] = {37, 13, 16, 40, 23, 17, 22, 19};
int negPintop[] = {41, 21, 20, 38, 18, 39, 14, 15};
// Pin configurations of 2nd Led matrix
int posPinbot[] = {9, 4, 29, 6, 10, 28, 11, 26};
int negPinbot[] = {5, 12, 25, 8, 27, 7, 3, 2};
// Initialize LEDMatrix instance
LEDMatrix LMtop(posPintop, 8, negPintop, 8);
LEDMatrix LMbot(posPinbot, 8, negPinbot, 8);

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
unsigned long interval = 500;
unsigned long prev = 0;
int lastButtonState = 0;

unsigned long lastRotateTime = 0;   // Timestamp for the last rotation
unsigned long rotateInterval = 300; // Interval for rotation (in milliseconds)

void genShape()
{
    if (!gotShape)
    {
        // Tetriminoes (shapes) in (y, x)
        int J[4][2] = {{0, 3}, {1, 1}, {0, 1}, {0, -1}};
        int L[4][2] = {{0, 3}, {1, -1}, {0, 1}, {0, -1}};
        int S[4][2] = {{0, 3}, {0, 1}, {1, 0}, {1, -1}};
        int Z[4][2] = {{0, 3}, {0, -1}, {1, 0}, {1, 1}};
        int T[4][2] = {{0, 3}, {0, 1}, {1, 0}, {1, -1}};
        int O[4][2] = {{0, 3}, {1, 0}, {0, 1}, {1, 1}};
        int I[4][2] = {{0, 3}, {0, 1}, {0, -1}, {0, -2}};

        // Array of pointers to tetrimino shapes
        int(*shapes[7])[2] = {J, L, S, Z, T, O, I};

        // Select a random shape
        int randomShapeIndex = random(0, 7);

        // Copy the shape into currentShape
        memcpy(currentShape, shapes[randomShapeIndex], 4 * 2 * sizeof(int));

        gotShape = true;
        alterShape(-1);
    }
}

void alterShape(int req)
{
    bool shiftable = true; // Initialize outside the switch block

    // Get all the coordinates
    for (int i = 0; i < 4; i++)
    {
        if (i == 0)
        {
            shapeCoordinates[i][0] = currentShape[i][0];
            shapeCoordinates[i][1] = currentShape[i][1];
        }
        else
        {
            shapeCoordinates[i][0] = currentShape[0][0] + currentShape[i][0];
            shapeCoordinates[i][1] = currentShape[0][1] + currentShape[i][1];
        }
    }

    switch (req)
    {
    case 0: // Record all shape coordinates to the display grid
        for (int i = 0; i < 4; i++)
        {
            stableMemory[shapeCoordinates[i][0]][shapeCoordinates[i][1]] = 1;
        }
        break;

    case 1:               // Shifting (left/right)
        shiftable = true; // Reset shiftable variable
        for (int i = 0; i < 4; i++)
        { // Check each section
            if (!(shapeCoordinates[i][1] + command > -1 && shapeCoordinates[i][1] + command < width))
            {                      // If it is not in range
                shiftable = false; // Not shiftable
                break;
            }
        }
        if (shiftable)
        {
            currentShape[0][1] = currentShape[0][1] + command; // Shift if possible
        }
        break;

    case 2: // Move downwards
        for (int i = 0; i < 4; i++)
        { // Check for each section of the shape
            if (stableMemory[shapeCoordinates[i][0] + 1][shapeCoordinates[i][1]] == 1 || shapeCoordinates[i][0] + 1 == checkingHeight)
            { // If one section's below is already registered as 1 or the section hits the ground
                alterShape(0);
                gotShape = false; // Shape has been used up
                break;
            }
        }

        if (gotShape)
        {
            currentShape[0][0]++;
        }
        else
        {
            scanAndClearGrid();
        }
        break;

    case 3: // rotating
        int tempRelative[3][2];
        for (int i = 0; i < 3; i++)
        {
            tempRelative[i][0] = currentShape[i + 1][0];
            tempRelative[i][1] = currentShape[i + 1][1];
        }

        bool rotate = true;
        for (int i = 1; i < 4; i++) // Flip and negate the relative coordinates for a 90-degree clockwise rotation
        {
            int temp = currentShape[i][0];
            currentShape[i][0] = currentShape[i][1];
            currentShape[i][1] = -temp;
        }
        alterShape(-1);
        for (int i = 0; i < 4; i++) // check each section
        {
            if (stableMemory[currentShape[i][0]][currentShape[i][1]] == 1)
            {
                rotate = false; // shape cannot be rotated
            }
        }
        if (!rotate)
        {
            for (int i = 1; i < 4; i++)
            {
                currentShape[i][0] = tempRelative[i - 1][0];
                currentShape[i][1] = tempRelative[i - 1][1];
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
}

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
}

void scanAndClearGrid()
{

    for (int i = 0; i < 3; i++) // checking at illegal zone, LOSING FACTOR
    {
        for (int j = 0; j < width; j++) // Check the rest of the row
        {
            if (stableMemory[i][j] == 1) // If any cell is full
            {
                end = true; // The row is not full
                return;     // Exit the function early
            }
        }
    }

    int fullRows[4] = {-1, -1, -1, -1}; // Array to store the indices of full rows initialized to -1
    int arrayCount = -1;

    for (int i = 3; i < checkingHeight; i++)
    {
        bool isFull = true; // Assume the row is full initially

        for (int j = 0; j < width; j++) // Check the rest of the row
        {
            if (stableMemory[i][j] == 0) // If any cell is empty
            {
                isFull = false; // The row is not full
                break;          // Exit the loop early
            }
        }

        if (isFull) // If the row is full
        {
            arrayCount++;
            fullRows[arrayCount] = i; // Store the row index
        }
    }

    // Counter for index of full rows
    if (arrayCount != -1) // if 1st element of fullRows is altered
    {
        for (int i = 0; i < arrayCount + 1; i++) // repeat the amount of rows to be cleared
        {
            for (int j = 0; j < width; j++)
            {
                stableMemory[fullRows[i]][j] = 0; // Clear the full row
            }
        }

        for (int i = fullRows[0]; i > 0; i--) // bring the rest of the rows downwards
        {
            for (int j = 0; j < width; j++)
            {
                stableMemory[i][j] = stableMemory[i - 1][j];
            }
        }

        if (scoring) // if scoring is enabled
        {
            int clearedRows = arrayCount + 1;         // rows cleared
            score += (pow(clearedRows, clearedRows)); // add the score
            Serial.println(score);                    // send score
        }
    }
}

void gatherThenShowDisplay(bool skip = false)
{
    // gather output
    if (!skip)
    {
        memcpy(displayMemory, stableMemory, sizeof(stableMemory));
        for (int i = 0; i < 4; i++)
        {
            displayMemory[shapeCoordinates[i][0]][shapeCoordinates[i][1]] = 1;
        }
    }

    // outputdisplay onto matrix
    for (int i = 0; i < checkingHeight - 2; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (i < width) // update the top LM
            {
                topLM[i][j] = displayMemory[i + 2][j];
            }
            else // update the bottom LM
            {
                botLM[i - width][j] = displayMemory[i + 2][j];
            }
        }
    }
}

void EndorRun()
{
    if (!end) // if not ending
    {
        if (millis() - prev < interval) // refresh according to interval
        {
            LMtop.Symbol(topLM, 2);
            LMbot.Symbol(botLM, 2);
        }
        else
        {
            prev = millis();
        }
    }
    else // ending
    {
        // for (int i = 0; i < 5; i++)  // blink the entire display 5 times
        // {
        //   LMtop.Symbol(topLM, 4);
        //   LMbot.Symbol(botLM, 4);
        // }

        // for (int i = height - 1; i > -1; i--)  // delete and show the entire display
        // {
        //   for (int j = width - 1; j > -1; j--)  // Correct loop counter
        //   {
        //     displayMemory[i][j] = 0;
        //     gatherThenShowDisplay(true);
        //   }
        // }
    }
}

void setup()
{
    randomSeed(analogRead(24));
    Serial.begin(500);

    for (int i = 30; i < 34; i++)
    {
        pinMode(i, INPUT);
    }
}

unsigned long lastInputTime = 0;
unsigned long inputInterval = 100; // Adjust this based on how frequently you want to check input

void loop()
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
        EndorRun();
    }
    else
    {
        Serial.println(score);
    }
}

// add in speed up system

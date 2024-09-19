#include <Dis7Seg.h> //if have 7-segment display liabrary
int score = 0;
bool scoring = true;

Dis7Seg dis('-', segmentPins, 4, digitPins);
int digitPins[] = {10, 11, 12, 13};
int segmentPins[] = {2, 3, 4, 5, 6, 7, 8, 9};
int scoreOut[] = {-1, -1, -1, 0};

#define ROTATE_PIN 34
#define LEFT_PIN 35
#define RIGHT_PIN 36
#define DOWN_PIN 37
#include <LEDMatrix.h>

// Initialize LEDMatrix instance
LEDMatrix LMtop(posPintop, 8, negPintop, 8);
LEDMatrix LMbot(posPinbot, 8, negPinbot, 8);

// Pin configurations of 1st Led matrix
const int posPintop[] = {2, 3, 4, 5, 6, 7, 8, 9};
const int negPintop[] = {10, 11, 12, 13, 14, 15, 16, 17};
// Pin configurations of 2nd Led matrix
const int posPinbot[] = {18, 19, 20, 21, 22, 23, 24, 25};
const int negPinbot[] = {26, 27, 28, 29, 30, 31, 32, 33};

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

// determinants
bool gotShape = false;
int command = -2;
int shapeCoordinates[4][2]; // in (y, x)
bool end = false;

void genShape()
{
    if (!gotShape)
    {
        // Tetriminoes (shapes) in (x, y)
        int J[4][2] = {{3, 3}, {1, 0}, {-1, 0}, {1, -1}};
        int L[4][2] = {{3, 3}, {1, 0}, {-1, 0}, {1, 1}};
        int S[4][2] = {{3, 3}, {1, 0}, {-1, 0}, {-1, -1}};
        int Z[4][2] = {{3, 3}, {-1, 0}, {-1, 0}, {1, -1}};
        int T[4][2] = {{3, 3}, {1, 0}, {-1, 0}, {0, -1}};
        int O[4][2] = {{3, 3}, {-1, 0}, {1, 0}, {-1, -1}};
        int I[4][2] = {{3, 3}, {0, 1}, {0, -1}, {0, -2}};

        // Array of pointers to tetrimino shapes
        int(*shapes[7])[2] = {J, L, S, Z, T, O, I};

        // Select a random shape
        int randomShapeIndex = random(0, 7);

        // Copy the shape into currentShape
        memcpy(currentShape, shapes[randomShapeIndex], 4 * 2 * sizeof(int));
    }
}

void requests(int req)
{
    switch (req)
    {
    default: // get all the coordinates
        for (int i = 0; i < 4; i++)
        {
            if (i == 0)
            {
                shapeCoordinates[i][1] = currentShape[i][0];
                shapeCoordinates[i][0] = currentShape[i][1];
            }
            else
            {
                shapeCoordinates[i][1] = currentShape[0][0] + currentShape[i][0];
                shapeCoordinates[i][0] = currentShape[0][1] + currentShape[i][1];
            }
        }
        break;

    case 0: // record all shape cords to the display grid
        for (int i = 0; i < 4; i++)
        {
            stableMemory[shapeCoordinates[i][0]][shapeCoordinates[i][1]] = 1;
        }
        break;

    case 1: // checking at the illegal zone
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < width; j++) // Check the rest of the row
            {
                if (stableMemory[i][j] == 1) // If any cell is full
                {
                    end = true; // The row is not full
                    break;      // Exit the loop early
                }
            }
        }
        break;

    case 2:                 // checking for full rows
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
        break;

    case 3:
        for (int i = 0; i < arrayCount + 1; i++) // repeat the amount of rows to be cleared
        {
            for (int j = 0; j < width; j++)
            {
                stableMemory[fullRows[i]][j] = 0; // Clear the full row
            }
        }

        for (int i = fullRows[0]; i > 0; i--)
        {
            for (int j = 0; j < width; j++)
            {
                stableMemory[i][j] = stableMemory[i - 1][j]; // bring the rest of the rows downwards
            }
        }
        break;
    }
}
void alterShape()
{
    switch (command)
    {
    case 0:
        request();
        for (int i = 0; i < 4; i++) // cheack for each section of the shape
        {
            if (stableMemory[shapeCoordinates[i][0] + 1][0] == 1 || shapeCoordinates[i][0] + 1 == checkingHeight - 1) // if one section's below is already registered as 1 or the section hits the ground
            {
                request(0);
                break;
            }
        }
        gotShape = false; // shape has been used up
        break;            // exit the loop early

    case (1 || -1):
        request();
        bool shiftable = true;      // can it be shifted
        for (int i = 0; i < 4; i++) // check each section
        {
            if (!(shapeCoordinates[i][1] + command > -1 && shapeCoordinates[i][1] + command < width)) // if it is not in range
            {
                shiftable = false; // not shitable
                break;
            }
        }
        if (shiftable)
        {
            currentShape[0][0] = currentShape[0][0] + command; // shift if possible
        }
        break;

    case 2:
        int tempRelative[3][2];
        for (int i = 0; i < 3; i++)
        {
            tempRelative[i][0] = currentShape[i + 1][0];
            tempRelative[i][1] = currentShape[i + 1][1];
        }

        bool rotate = true;
        for (int i = 1; i < 4; i++)
        {
            // Flip and negate the relative coordinates for a 90-degree clockwise rotation
            int temp = currentShape[i][0];
            currentShape[i][0] = currentShape[i][1];
            currentShape[i][1] = -temp;
        }
        req();
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
    return;
};

void checkInput(int forced = false) // check and identify the input command
{
    bool leftState = digitalRead(LEFT_PIN) == HIGH;
    bool rightState = digitalRead(RIGHT_PIN) == HIGH;
    bool downState = digitalRead(DOWN_PIN) == HIGH;
    bool rotateState = digitalRead(ROTATE_PIN) == HIGH;

    if (downState || forced)
    {
        command = 0;
    }
    else if (rotateState)
    {
        command = 2;
    }
    else if (leftState)
    {
        command = -1;
    }
    else if (rightState)
    {
        command = 1;
    }
    else
    {
        command = -2;
    }
}

void scanAndClearGrid()
{
    int fullRows[4] = {-1, -1, -1, -1}; // Array to store the indices of full rows initialized to -1
    int arrayCount = -1;                // Counter for index of full rows

    for (int i = 0; i < checkingHeight; i++)
    {
        if (i < 3)
        {
            req(1);
        }
        else
        {
            req(2);
        }
    }

    if (arrayCount != -1) // if 1st element of fullRows is altered
    {
        req(3);
        if (scoring) // if scoring is enabled
        {
            int clearedRows = arrayCount + 1;                  // rows cleared
            int score += pow(clearedRows, clearedRows); // add the score
        }
    }
}

void gatherDisplay()
{
    memset(displayMemory, stableMemory, witdh * checkingHeight);

    for (int i = 0; i < 4; i++) // light the moving shape
    {
        if (i == 0)
        {
            displayMemory[currentShape[i][0]][currentShape[i][1]] = 1;
        }
        else
        {
            displayMemory[currentShape[0][0] + currentShape[i][0]][currentShape[0][1] + currentShape[i][1]] = 1;
        }
    }
}

void showDisplay()
{
    for (int i = 0; i < height; i++)
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
        unsigned long interval = 200;
        unsigned long prev = millis();
        while (millis() - prev < interval) // refresh according to interval
        {
            LMtop.Symbol(topLM);
            LMbot.Symbol(botLM);
            dis.scan(scoreOut);
        }
    }
    else // ending
    {
        for (int i = 0; i < 5; i++) // blink the entire display 5 times
        {
            LMtop.Symbol(topLM);
            LMbot.Symbol(botLM);
            dis.scan(scoreOut);
        }

        for (int i = height - 1; i > -1; i--) // delete and show the entire display
        {
            for (int j = width - 1; j > -1; j--) // Correct loop counter
            {
                displayMemory[i][j] = 0;
                showDisplay();
                dis.scan(scoreOut);
            }
        }
    }
}

void ending()
{
    dis.scan();
}

void setup()
{
    randomSeed(analogRead(A4));
 
}

void loop()
{
    if (!end)
    {
        genShape();

        checkInput();
        stabilizeShape();
        checkInput(true);
        stabilizeShape();

        scanAndClearGrid();
        gatherDisplay();
        showDisplay();
        EndorRun();
    }
    else
    {
        ending();
    }
}

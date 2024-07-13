#define rotate 1
#define left 2
#define right 3
#define down 4
#include <LEDMatrix.h>

// Pin configurations of 1st Led matrix
int posPintop[] = {2, 3, 4, 5, 6, 7, 8, 9};
int negPintop[] = {10, 11, 12, 13, A0, A1, A2, A3};
// Pin configurations of 2nd Led matrix
int posPinbot[] = {2, 3, 4, 5, 6, 7, 8, 9};
int negPinbot[] = {10, 11, 12, 13, A0, A1, A2, A3};

// Initialize LEDMatrix instance
LEDMatrix LMtop(posPintop, 8, negPintop, 8);
LEDMatrix LMbot(posPinbot, 8, negPinbot, 8);

// Shape definition
const int height = 16;
const int width = 8;

int displayMemory[height][width] = {{0}};
int stableMemory[height][width] = {{0}};

int topLM[width][width];
int botLM[width][width];

int (*shapes[7])[4][2] = {&L, &J, &S, &Z, &T, &O, &I};

int J[4][2] = {{3, 15}, {1, 0}, {-1, 0}, {1, 1}};
int L[4][2] = {{3, 15}, {1, 0}, {-1, 0}, {1, -1}};
int S[4][2] = {{3, 15}, {1, 0}, {-1, 0}, {-1, -1}};
int Z[4][2] = {{3, 15}, {-1, 0}, {-1, 0}, {1, -1}};
int T[4][2] = {{3, 15}, {1, 0}, {-1, 0}, {0, -1}};
int O[4][2] = {{3, 15}, {-1, 0}, {1, 0}, {-1, -1}};
int I[4][2] = {{3, 15}, {0, 1}, {0, -1}, {0, -2}};

char command = '\0';
bool gotShape = false;
int currentShape[4][2] = {{0}};

void genShape()
{
    if (!gotShape)
    {
        int(*selectedShape)[4][2] = shapes[random(0, 7)];
        for (int i = 0; i < 4; i++)
        {
            currentShape[i][0] = (*selectedShape)[i][0];
            currentShape[i][1] = (*selectedShape)[i][1];
        }
        gotShape = true;
    }
}

void checkInput()
{
    bool leftState = digitalRead(left) == HIGH;
    bool rightState = digitalRead(right) == HIGH;
    bool downState = digitalRead(down) == HIGH;
    bool rotateState = digitalRead(rotate) == HIGH;

    if (rotateState)
    {
        command = 'o';
    }
    else if (downState)
    {
        command = 'd';
    }
    else if (leftState)
    {
        command = 'l';
    }
    else if (rightState)
    {
        command = 'r';
    }
    else
    {
        command = '\0';
    }
}

void checkAndAlterShape()
{
    if (command == 'o')
    {
        for (int i = 1; i < 4; i++)
        {
            int rmb = currentShape[i][0];
            currentShape[i][0] = currentShape[i][1] * -1;
            currentShape[i][1] = rmb * -1;
        }
    }
    else if (command == 'l')
    {
        currentShape[0][0]--;
    }
    else if (command == 'r')
    {
        currentShape[0][0]++;
    }
    else if (command == 'd')
    {
        currentShape[0][1]--;
    }
    command = '\0';
}

void scanAndClearGrid()
{
    int fullRows[4];     // Array to store the indices of full rows initialized to -1
    int arrayCount = -1; // Counter for index of full rows

    for (int i = height; i > 0; i--)
    {
        if (stableMemory[i][0] != 0) // Check if the first cell of the row is not empty
        {
            bool isFull = true; // Assume the row is full initially

            for (int j = 1; j < width; j++) // Check the rest of the row
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
                fullRow[arrayCount] = i; // Store the row index
            }
        }
    }

    if (arrayCount != -1) // if 1st element of fullRows is altered
    {
        for (int i = 0; i < arrayCount + 1; i++) // repeat the amount of rows to be cleared
        {
            for (int j = 0; j < width; j++)
            {
                stableMemory[fullRow[i]][j] = 0; // Clear the full row
                if (fullRow[i] < width)          // if index less than width
                {                                // top half
                    LMtop.OnRow(i);              // animation
                    LMtop.Symbol(topLM);
                }
                else                        // else index is more than width
                {                           // bottom half
                    LMbot.OnRow(i - width); // animation
                    LMbot.Symbol(botLM);
                }
            }
        }

        for (int i = fullRow[0]; i > 0; i--)
        {
            for (int j = 0; j < width; j++)
            {
                stableMemory[i][j] = stableMemory[i - (arrayCount + 1)][j]; // bring the rest of the rows downwards
            }
        }
    }
};

void displayUpdate()
{
    for (int i = 0; i < height; i++) // clear grid
    {
        for (int j = 0; j < width; j++)
        {
            displayMemory[i][j] = 0;
        }
    }

    for (int i = 0; i < height; i++) // light all existing stable coordinates
    {
        for (int j = 0; j < width; j++)
        {
            if (stableMemory[i][j] == 1)
            {
                displayMemory[i][j] = 1;
            }
        }
    }

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
};

void showDisplay()
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (i < width) // update the top LM
            {
                topLM[i][j] = displayMemory[i][j];
            }
            else // update the bottom LM
            {
                botLM[i - width][j - width] = displayMemory[i][j];
            }
        }
    }
}

void setup()
{
    randomSeed(analogRead(A4));
}

void loop()
{
    checkInput();
    checkAndAlter();
    genShape();
    MemtoDisplay();
}

#define rotate 1
#define left 2
#define right 3
#define down 4

int memory[16][8] = {{0}};
int topLM[8][8];
int botLM[8][8];
int height = 0;

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

void checkAndAlter()
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

void scanGrid()
{
    int fullRow[];
    int count = 0;
    int arrayCount = 0;
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (memory[i][j] == 0)
            {
                count++;
            }
            if (count == 8)
            {
                fullRow[arrayCount] = i;
                arrayCount++;
            }
        }
    }
}

// void scanGrid() {
int count = 0;
bool row[15] = {0};
int c = 0;
for (int i = 0; i < height; i++)
{
    for (int j = 0; j < 8; j++)
    {
        if (memory[i][j] == 1)
        {
            // Placeholder for functionality
        }
    }
}
}

void MemtoDisplay()
{
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (i < 8)
            {
                topLM[i][j] = memory[i][j];
            }
            else
            {
                botLM[i - 8][j] = memory[i][j];
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
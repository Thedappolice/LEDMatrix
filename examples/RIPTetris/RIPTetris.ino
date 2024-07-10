
#define rotate 1
#define left 2
#define right 3
#define down 4

int memory[16][8] = {{0}};
int topLM[8][8];
int botLM[8][8];

int (*shapes[7])[4][2] = {&L, &J, &S, &Z, &T, &O, &I};

int J[4][2] = {{3, 15}, {1, 0}, {-1, 0}, {1, 1}};
int L[4][2] = {{3, 15}, {1, 0}, {-1, 0}, {1, -1}};
int S[4][2] = {{3, 15}, {1, 0}, {-1, 0}, {-1, -1}};
int Z[4][2] = {{3, 15}, {-1, 0}, {-1, 0}, {1, -1}};
int T[4][2] = {{3, 15}, {1, 0}, {-1, 0}, {0, -1}};
int O[4][2] = {{3, 15}, {-1, 0}, {1, 0}, {-1, -1}};
int I[4][2] = {{3, 15}, {0, 1}, {0, -1}, {0, -2}};

char command;

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
    }
};

void checkBound(char command)
{
    if (command == 'u')
    {
    }
    else if (command == 'r')
    {
    }
};

void checkInput()
{
    bool leftstate = digitalRead(left) == HIGH;
    bool rightstate = digitalRead(right) == HIGH;
    bool downstate = digitalRead(down) == HIGH;
    bool rotatestate = digitalRead(rotate) == HIGH;

    if (rotatestate)
    {
        command = 'r';
    }
    else if (downstate)
    {
        command = 'd';
    }
    else if (leftstate)
    {
        command = 'l';
    }
    else if (rightstate)
    {
        command = 'r';
    }
};

MemtoDisplay()
{
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (i < 7)
            {
                topLM[i][j] = memory[2 * i][2 * j];
            }
            else
            {
                botLM[i][j] = memory[2 * i][2 * j];
            }
        }
    }
};

void setup()
{
    randomSeed(analogRead(A4));
}

void loop()
{
}

#define rotate 1
#define left 2
#define right 3
#define down 4

int memory[31][15] = {{0}};
int topLM[8][8];
int botLM[8][8];

char shapes[7] = {'L', 'J', 'S', 'Z', 'T', 'O', 'I'};

char command;

bool gotShape = false;
char shape[4][2] = {{0}};
int orientation = 1;

void genShape()
{
    if (gotShape)
    {
        shape = shapes[random(0, 6)];
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
for(int i = 0; i <16 ; i++)
{
for(int j = 0; j < 8; j++)
{
if(i<7)
{
topLM[i][j]=memory[2*i][2*j];
}
else
{
botLM[i][j]=memory [2*i][2*j];
}
}
}
};

void setup()
{
}

void loop()
{
}
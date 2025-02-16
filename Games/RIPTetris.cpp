#include "RIPTetris.h"

// Constructor
RIPTetris::RIPTetris(LEDMatrix<8, 8> &topDisplay, LEDMatrix<8, 8> &botDisplay)
{
    topLM = &topDisplay;
    botLM = &botDisplay;
}

// Destructor
RIPTetris::~RIPTetris()
{
}

// Game Loop
void RIPTetris::gameLoop()
{
    static unsigned long lastDropTime = 0;
    static unsigned long lastMoveTime = 0;
    unsigned long currentTime = millis();

    if (!end)
    {
        checkInput();

        if (currentTime - lastDropTime >= interval)
        {
            lastDropTime = currentTime;
            alterShape(0); // Move shape down automatically
        }

        if (currentTime - lastMoveTime >= inputInterval)
        {
            lastMoveTime = currentTime;
            checkInput();
        }

        gatherAndDisplay();
    }
    else
    {
        showEndAnimation();
        ended = true;
    }
}

// Generate New Shape
void RIPTetris::genShape()
{
    const int shapes[7][4][2] = {
        {{0, 0}, {0, 1}, {1, 0}, {1, 1}},
        {{0, 0}, {0, 1}, {0, 2}, {0, 3}},
        {{0, 0}, {1, 0}, {1, 1}, {1, 2}},
        {{0, 0}, {1, 0}, {0, 1}, {0, 2}},
        {{0, 0}, {0, 1}, {1, 1}, {1, 2}},
        {{0, 0}, {0, 1}, {1, 0}, {1, -1}},
        {{0, 0}, {0, 1}, {1, 0}, {0, -1}}};

    int randomIndex = random(0, 7);
    memcpy(shape, shapes[randomIndex], sizeof(shape));

    for (int i = 0; i < 4; i++)
    {
        coordinates[i][0] = shape[i][0] + 1;
        coordinates[i][1] = shape[i][1] + GRID_WIDTH / 2;
    }

    active = true;
}

// Handle Shape Movement & Rotation
void RIPTetris::alterShape(int mode)
{
    int newCoordinates[4][2];

    for (int i = 0; i < 4; i++)
    {
        newCoordinates[i][0] = coordinates[i][0];
        newCoordinates[i][1] = coordinates[i][1];

        if (mode == -1)
            newCoordinates[i][1] -= 1;
        else if (mode == 1)
            newCoordinates[i][1] += 1;
        else if (mode == 0)
            newCoordinates[i][0] += 1;
        else if (mode == 3)
        {
            int pivotX = coordinates[0][1];
            int pivotY = coordinates[0][0];

            for (int j = 0; j < 4; j++)
            {
                int relX = coordinates[j][1] - pivotX;
                int relY = coordinates[j][0] - pivotY;

                newCoordinates[j][0] = pivotY - relX;
                newCoordinates[j][1] = pivotX + relY;
            }
        }
    }

    memcpy(coordinates, newCoordinates, sizeof(newCoordinates));
}

// Check Input
void RIPTetris::checkInput()
{
    if (digitalRead(ROTATE_PIN) == HIGH && previousRotateState == LOW)
    {
        alterShape(3);
    }
    previousRotateState = digitalRead(ROTATE_PIN);

    int Xvalue = analogRead(LEFT_OR_RIGHT_PIN);
    int Yvalue = analogRead(UP_OR_DOWN_PIN);

    if (Xvalue > 900)
        alterShape(1);
    if (Xvalue < 100)
        alterShape(-1);
    if (Yvalue < 100)
        alterShape(0);
}

// Scan & Clear Rows
void RIPTetris::scanAndClearGrid()
{
    int clearedRows = 0;

    for (int i = CHECKING_HEIGHT - 1; i >= 3; i--)
    {
        bool isFull = true;
        for (int j = 0; j < GRID_WIDTH; j++)
        {
            if (displayMatrix[i][j] == 0)
            {
                isFull = false;
                break;
            }
        }

        if (isFull)
        {
            memset(displayMatrix[i], 0, sizeof(displayMatrix[i]));
            clearedRows++;
        }
    }

    const int scoreTable[] = {0, 40, 100, 300, 1200};
    score += scoreTable[clearedRows];

    if (score > 9999)
        score = 9999;

    if (clearedRows > 0)
    {
        interval = max(100, interval - 50);
    }

    sendScore(score);
}

// Display Updates
void RIPTetris::gatherAndDisplay()
{
    for (int i = 0; i < GRID_WIDTH; i++)
    {
        memcpy(displayMatrix[i], &displayMatrix[i + 2][0], sizeof(displayMatrix[i]));
    }

    topLM->Symbol(displayMatrix);
    botLM->Symbol(displayMatrix);
}

// Show End Animation
void RIPTetris::showEndAnimation()
{
    for (int i = CHECKING_HEIGHT; i > 0; i--)
    {
        memset(displayMatrix[i], 0, sizeof(displayMatrix[i]));
        gatherAndDisplay();
        delay(50);
    }
}

// Send Score
void RIPTetris::sendScore(int score)
{
    Serial5.write(lowByte(score));
    Serial5.write(highByte(score));
}

#include "RIPTetris.h"

// Constructor
RIPTetris::RIPTetris()
{
}

// Destructor
RIPTetris::~RIPTetris()
{
}

// Game Loop
void RIPTetris::gameLoop(direction input)
{
    static unsigned long lastDropTime = 0;
    static unsigned long lastMoveTime = 0;
    unsigned long currentTime = millis();

    if (!end)
    {
        if (!active)
            genShape();
        if (active)
        {
            checkInput(input);

            if (currentTime - lastDropTime >= interval)
            {
                lastDropTime = currentTime;
                alterShape(0); // Move shape down automatically
            }

            if (currentTime - lastMoveTime >= inputInterval)
            {
                lastMoveTime = currentTime;
                checkInput(DOWN);
            }
        }
    }

    // for every update call, remeber to call gameDisplay matrix
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
        ShapeCoordinates[i][0] = shape[i][0] + 1;
        ShapeCoordinates[i][1] = shape[i][1] + GRID_WIDTH / 2;
    }

    if (!validPosition(ShapeCoordinates))
    {
        end = true;
        return;
    }

    active = true;
}

// Handle Shape Movement & Rotation
void RIPTetris::alterShape(int mode)
{
    int newCoordinates[4][2];

    for (int i = 0; i < 4; i++)
    {
        newCoordinates[i][0] = ShapeCoordinates[i][0];
        newCoordinates[i][1] = ShapeCoordinates[i][1];

        if (mode == -1)
        {
            newCoordinates[i][1] -= 1;
        }
        else if (mode == 1)
        {
            newCoordinates[i][1] += 1;
        }
        else if (mode == 0)
        {
            for (int i = 0; i < 4; i++)
                newCoordinates[i][0] += 1; // move down

            if (validPosition(newCoordinates))
            {
                memcpy(ShapeCoordinates, newCoordinates, sizeof(newCoordinates));
                return;
            }
            else
            {
                // Place shape
                for (int i = 0; i < 4; i++)
                {
                    int y = ShapeCoordinates[i][0];
                    int x = ShapeCoordinates[i][1];
                    if (y >= 0 && y < GRID_HEIGHT && x >= 0 && x < GRID_WIDTH)
                        gameMatrix[y][x] = 1;
                    if (y >= GRID_HEIGHT)
                        end = true;
                }
                active = false;
                scanAndClearGrid();
                return;
            }
        }

        else if (mode == 3)
        {
            int rotated[4][2];
            getRotatedCoordinates(rotated);

            // Try wall kicks: no shift, left 1, right 1, left 2, right 2
            const int kicks[] = {0, -1, 1, -2, 2};
            for (int i = 0; i < 5; i++)
            {
                int kicked[4][2];
                for (int j = 0; j < 4; j++)
                {
                    kicked[j][0] = rotated[j][0];
                    kicked[j][1] = rotated[j][1] + kicks[i];
                }

                if (validPosition(kicked))
                {
                    memcpy(ShapeCoordinates, kicked, sizeof(kicked));
                    scanAndClearGrid();
                    return;
                }
            }

            // All kicks failed → no rotation
        }
    }

    if (validPosition(newCoordinates))
    {
        memcpy(ShapeCoordinates, newCoordinates, sizeof(newCoordinates));
        scanAndClearGrid();
    }
}

void RIPTetris::getRotatedCoordinates(int output[4][2])
{
    int pivotX = ShapeCoordinates[0][1];
    int pivotY = ShapeCoordinates[0][0];

    for (int j = 0; j < 4; j++)
    {
        int relX = ShapeCoordinates[j][1] - pivotX;
        int relY = ShapeCoordinates[j][0] - pivotY;
        output[j][0] = pivotY - relX;
        output[j][1] = pivotX + relY;
    }
}

bool RIPTetris::validPosition(int coords[4][2])
{
    for (int i = 0; i < 4; i++)
    {
        int y = coords[i][0];
        int x = coords[i][1];
        if (x < 0 || x >= GRID_WIDTH || y < 0 || y >= CHECKING_HEIGHT)
            return false;
        if (gameMatrix[y][x] != 0)
            return false;
    }
    return true;
}

// Check Input
void RIPTetris::checkInput(direction input)
{
    switch (input)
    {
    case DOWN:
        alterShape(0);
        break;
    case LEFT:
        alterShape(-1);
        break;
    case RIGHT:
        alterShape(1);
        break;
    case ROTATE:
        alterShape(3);
        break;
    }
}

// Scan & Clear Rows
void RIPTetris::scanAndClearGrid()
{
    int clearedRows = 0;

    for (int i = CHECKING_HEIGHT - 1; i >= 2; i--)
    {
        bool isFull = true;
        for (int j = 0; j < GRID_WIDTH; j++)
        {
            if (gameMatrix[i][j] == 0)
            {
                isFull = false;
                break;
            }
        }

        if (isFull)
        {
            for (int k = i; k > 0; k--)
            {
                memcpy(gameMatrix[k], gameMatrix[k - 1], sizeof(gameMatrix[k]));
            }
            memset(gameMatrix[0], 0, sizeof(gameMatrix[0])); // Top row is now empty
            clearedRows++;
            i++; // Recheck the same row index after shifting
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
}

void RIPTetris::getDisplayMatrix(int outMatrix[GRID_HEIGHT][GRID_WIDTH])
{
    // Copy placed blocks first
    for (int y = 0; y < GRID_HEIGHT; y++)
    {
        for (int x = 0; x < GRID_WIDTH; x++)
        {
            outMatrix[y][x] = gameMatrix[y][x];
        }
    }

    // Overlay the current falling shape
    if (active)
    {
        for (int i = 0; i < 4; i++)
        {
            int y = ShapeCoordinates[i][0];
            int x = ShapeCoordinates[i][1];
            if (y >= 0 && y < GRID_HEIGHT && x >= 0 && x < GRID_WIDTH)
                outMatrix[y][x] = 1;
        }
    }
}

// // Show End Animation
// void RIPTetris::showEndAnimation()
// {
//     for (int i = CHECKING_HEIGHT; i > 0; i--)
//     {
//         memset(gameMatrix[i], 0, sizeof(gameMatrix[i]));
//         gatherAndDisplay();
//         delay(50);
//     }
// }

// // Send Score
// void RIPTetris::sendScore(int score)
// {
//     Serial5.write(lowByte(score));
//     Serial5.write(highByte(score));
// }

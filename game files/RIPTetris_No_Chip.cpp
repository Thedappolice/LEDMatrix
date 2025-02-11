#include "RIPTetris_No_Chip.h"

// function declaration

Tetris_No_Chip::Tetris_No_Chip(LEDMatrix<8, 8> topDisplay, LEDMatrix<8, 8> botDisplay)
{
    topLM = new LEDMatrix<8, 8>();
    botLM = new LEDMatrix<8, 8>();

    topLM = topDisplay;
    botLM = botDisplay;
}

Tetris_No_Chip::~Tetris_No_Chip()
{
}

void Tetris_No_Chip::Symbol( char input, unsigned long duration)
{
    // Predefined symbols
    int symbols[4][8][8] = {
        {// Symbol '1'
         {0, 0, 0, 1, 1, 0, 0, 0},
         {0, 0, 1, 1, 1, 0, 0, 0},
         {0, 1, 1, 1, 1, 0, 0, 0},
         {0, 0, 0, 1, 1, 0, 0, 0},
         {0, 0, 0, 1, 1, 0, 0, 0},
         {0, 0, 0, 1, 1, 0, 0, 0},
         {0, 1, 1, 1, 1, 1, 1, 0},
         {0, 1, 1, 1, 1, 1, 1, 0}},
        {// Symbol '2'
         {0, 0, 1, 1, 1, 1, 0, 0},
         {0, 1, 1, 1, 1, 1, 1, 0},
         {0, 1, 1, 0, 0, 1, 1, 0},
         {0, 0, 0, 0, 1, 1, 1, 0},
         {0, 0, 0, 1, 1, 1, 0, 0},
         {0, 0, 1, 1, 1, 0, 0, 0},
         {0, 1, 1, 1, 1, 1, 1, 0},
         {0, 1, 1, 1, 1, 1, 1, 0}},
        {// Symbol '3'
         {0, 0, 1, 1, 1, 1, 0, 0},
         {0, 1, 1, 1, 1, 1, 1, 0},
         {0, 0, 0, 0, 0, 1, 1, 0},
         {0, 0, 1, 1, 1, 1, 0, 0},
         {0, 0, 1, 1, 1, 1, 0, 0},
         {0, 0, 0, 0, 0, 1, 1, 0},
         {0, 1, 1, 1, 1, 1, 1, 0},
         {0, 0, 1, 1, 1, 1, 0, 0}},
        {// Symbol 'E' (End)
         {0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0},
         {1, 1, 1, 0, 0, 0, 0, 1},
         {1, 0, 0, 0, 0, 0, 0, 1},
         {1, 1, 0, 0, 1, 0, 0, 1},
         {1, 0, 0, 1, 0, 1, 1, 0},
         {1, 1, 1, 1, 0, 1, 0, 1},
         {0, 0, 0, 0, 0, 0, 0, 0}}};

    // Map the input character to the symbol index
    int symbolIndex = -1;
    switch (input)
    {
    case '1':
        symbolIndex = 0;
        break;
    case '2':
        symbolIndex = 1;
        break;
    case '3':
        symbolIndex = 2;
        break;
    case 'E':
        symbolIndex = 3;
        break;
    default:
        break;
    }

    memcpy(displayMatrix, symbols[symbolIndex], sizeof(displayMatrix));

    gatherAndDisplay(0);
}

void alterShape(int mode)
{
    int newCoordinates[4][2]; // Temporary coordinates for validation

    // Calculate the new positions
    for (int i = 0; i < 4; i++)
    {
        newCoordinates[i][0] = currentShape.coordinates[i][0];
        newCoordinates[i][1] = currentShape.coordinates[i][1];

        if (mode == -1) // Move left
            newCoordinates[i][1] -= 1;
        else if (mode == 1) // Move right
            newCoordinates[i][1] += 1;
        else if (mode == 0) // Move down
            newCoordinates[i][0] += 1;
        else if (mode == 3) // Rotate
        {
            int y = currentShape.coordinates[0][0];
            int x = currentShape.coordinates[0][1];
            newCoordinates[i][0] = y - (currentShape.coordinates[i][1] - x);
            newCoordinates[i][1] = x + (currentShape.coordinates[i][0] - y);
        }
    }

    // Validate the new positions
    bool validMove = true;
    for (int i = 0; i < 4; i++)
    {
        int y = newCoordinates[i][0];
        int x = newCoordinates[i][1];

        if (y >= CHECKING_HEIGHT || x < 0 || x >= GRID_WIDTH || grid.stable[y][x])
        {
            validMove = false;
            break;
        }
    }

    // Apply the new positions if valid
    if (validMove)
    {
        memcpy(currentShape.coordinates, newCoordinates, sizeof(newCoordinates));

        if (mode == 0) // Handle locking when moving down
        {
            for (int i = 0; i < 4; i++)
            {
                if (currentShape.coordinates[i][0] == CHECKING_HEIGHT - 1 || grid.stable[currentShape.coordinates[i][0] + 1][currentShape.coordinates[i][1]])
                {
                    // Lock the shape into the stable grid
                    for (int j = 0; j < 4; j++)
                        grid.stable[currentShape.coordinates[j][0]][currentShape.coordinates[j][1]] = 1;

                    currentShape.active = false; // Deactivate the current shape
                    scanAndClearGrid();          // Check for and clear full rows
                    break;
                }
            }
        }
    }
}

void Tetris_No_Chip::clearRow(int row)
{
    memset(grid.stable[row], 0, sizeof(grid.stable[row]));
}

void Tetris_No_Chip::shiftRowsDown(int startRow)
{
    for (int i = startRow; i > 0; i--)
    {
        memcpy(grid.stable[i], grid.stable[i - 1], sizeof(grid.stable[i]));
    }
}

void Tetris_No_Chip::scanAndClearGrid()
{
    // Check for overflow in the illegal zone (top 3 rows)
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < GRID_WIDTH; j++)
        {
            if (grid.stable[i][j] == 1)
            {
                end = true; // Game over
                return;
            }
        }
    }

    int clearedRows = 0;

    // Check and clear full rows from bottom to top
    for (int i = CHECKING_HEIGHT - 1; i >= 3; i--)
    {
        bool isFull = true;
        for (int j = 0; j < GRID_WIDTH; j++)
        {
            if (grid.stable[i][j] == 0)
            {
                isFull = false;
                break;
            }
        }

        if (isFull)
        {
            clearRow(i); // Clear the full row
            clearedRows++;
        }
        else if (clearedRows > 0)
        {
            shiftRowsDown(i + clearedRows); // Shift rows down if rows were cleared
        }
    }

    // Update the score based on the number of cleared rows
    if (clearedRows > 0)
    {
        score += pow(clearedRows, clearedRows); // Exponential scoring based on rows cleared
        if (score > 9999)
        {
            score = 9999; // Cap the score at 9999
        }
        sendScore(score);
    }
}

void Tetris_No_Chip::checkInput()
{
}

void Tetris_No_Chip::gatherAndDisplay(bool symbol)
{
    if(symbol)
    {
        LMbot.Symbol(displayMatrix);
        return;
    }

    if (!end)
    { // Copy the stable grid into the display grid
        memcpy(grid.display, grid.stable, sizeof(grid.stable));

        // Add the active shape to the display grid
        for (int i = 0; i < 4; i++)
        {
            grid.display[currentShape.coordinates[i][0]][currentShape.coordinates[i][1]] = 1;
        }
    }

    // Update the top and bottom matrix displays
    for (int i = 0; i < GRID_WIDTH; i++)
    {
        memcpy(grid.top[i], &grid.display[i + 2][0], sizeof(grid.top[i]));                    // Top matrix
        memcpy(grid.bottom[i], &grid.display[i + GRID_WIDTH + 2][0], sizeof(grid.bottom[i])); // Bottom matrix
    }

    // Send updated data to the LED matrices
    LMtop.Symbol(grid.top, 2);
    LMbot.Symbol(grid.bottom, 2);
}

void Tetris_No_Chip::showEndAnimation()
{
    for (int i = CHECKING_HEIGHT; i > 0; i--)
    {
        for (int j = 0; j < GRID_WIDTH; j++)
        {
            grid.display[i][j] = 0; // Clear rows one by one
        }
        gatherAndDisplay();
        delay(50); // Small delay for animation effect
    }
}

void Tetris_No_Chip::sendScore(int score)
{
    Serial5.begin(9600); // Initialize Serial7 for score transmission

    // Initialize control pins as input
    pinMode(ROTATE_PIN, INPUT);

    randomSeed(analogRead(21)); // Seed the random number generator
    sendScore(10000);           // Signal game start to another board
}

void Tetris_No_Chip ::gameLoop()
{
    if (ended)
    {
        Char(LMbot, 'E', 2000); // Show the 'E' symbol for 2000 ms
    }
    else if (!end)
    {
        genShape();   // Generate a new shape if needed
        checkInput(); // Process player inputs

        // Handle automatic downward movement
        if (millis() - prev >= interval)
        {
            prev = millis();
            alterShape(0); // Move the shape down automatically
        }

        gatherAndDisplay(); // Update the display
    }
    else
    {
        showEndAnimation(); // Display the end animation
        ended = true;       // Mark the end animation as shown
    }
}

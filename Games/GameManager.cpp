#include "GameManager.h"

// Constructor
GameManager::GameManager(LEDMatrix<8, 8> &LMtop, LEDMatrix<8, 8> &LMbot)
{
    topLM = &LMtop;
    botLM = &LMbot;
}

// Destructor
GameManager::~GameManager()
{
    delete snake;
    delete tetris;
}

// Main update function
void GameManager::update()
{
    checkInput();

    if (game == Empty)
    {
        showMenu();
    }
    else
    {
        if (game == Snake)
        {
            snake->gameLoop();
        }
        else if (game == Tetris)
        {
            tetris->gameLoop();
        }

        // Check if reset condition is met
        resetGame();
    }
}

// Show menu selection
void GameManager::showMenu()
{
    int icons[2][8][8] =
        {
            {// Snake icon
             {0, 0, 0, 0, 0, 0, 0, 0},
             {0, 1, 1, 1, 1, 1, 1, 0},
             {0, 1, 1, 1, 1, 1, 1, 0},
             {0, 0, 0, 1, 1, 0, 0, 0},
             {0, 0, 0, 1, 1, 0, 0, 0},
             {0, 0, 0, 1, 1, 0, 0, 0},
             {0, 0, 0, 1, 1, 0, 0, 0},
             {0, 0, 0, 0, 0, 0, 0, 0}},

            {// Tetris icon
             {0, 0, 0, 0, 0, 0, 0, 0},
             {0, 0, 1, 1, 1, 1, 0, 0},
             {0, 1, 0, 0, 0, 0, 1, 0},
             {0, 0, 1, 1, 1, 0, 0, 0},
             {0, 0, 0, 0, 1, 1, 0, 0},
             {0, 1, 0, 0, 0, 0, 1, 0},
             {0, 0, 1, 1, 1, 1, 0, 0},
             {0, 0, 0, 0, 0, 0, 0, 0}}};

    memcpy(topDisplay, icons[0], sizeof(topDisplay));
    memcpy(botDisplay, icons[1], sizeof(botDisplay));

    int(&Ref)[8][8] = (cursor) ? topDisplay : botDisplay;

    // Draw border around the selected game
    for (int i = 0; i < 8; i++)
    {
        if (i == 0 || i == 7)
        {
            for (int j = 0; j < 8; j++)
            {
                Ref[i][j] = 1;
            }
        }
        else
        {
            Ref[i][0] = Ref[i][7] = 1;
        }
    }

    selection();
}

// Handle game selection
void GameManager::selection()
{
    if (rotate && !prevRotateState) // Prevent accidental double press
    {
        if (cursor)
        {
            if (!snake)
            {
                snake = new Joystick_snake(*botLM);
            }
            game = Snake;
        }
        else
        {
            if (!tetris)
            {
                tetris = new RIPTetris(*topLM, *botLM);
            }
            game = Tetris;
        }
    }

    prevRotateState = rotate;
}

// Check user input
void GameManager::checkInput()
{
    int Xvalue = analogRead(UP_OR_DOWN_PIN);
    int Yvalue = analogRead(LEFT_OR_RIGHT_PIN);
    bool Zvalue = digitalRead(ROTATE_PIN);

    rotate = Zvalue;

    if (Xvalue > 900)
        left_right = left;
    else if (Xvalue < 100)
        left_right = right;
    else
        left_right = neutral;

    if (Yvalue > 900)
        up_down = up;
    else if (Yvalue < 100)
        up_down = down;
    else
        up_down = neutral;
}

// Reset game and return to menu
void GameManager::resetGame()
{
    // If joystick is pushed up and rotate button is pressed, return to menu
    if (up_down == up && rotate)
    {
        if (!resetTrigger) // Prevents accidental continuous reset
        {
            delete snake;
            delete tetris;
            snake = nullptr;
            tetris = nullptr;
            game = Empty;
            resetTrigger = true; // Ensure it doesn't trigger again until released
        }
    }
    else
    {
        resetTrigger = false; // Reset trigger when buttons are released
    }
}

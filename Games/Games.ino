#include <RIPTetris_No_Chip.h>
#include <Joystick_snake_No_Chip.h>

#include <math.h> // Math library for functions like pow()

// Pin Definitions for Controls
#define ROTATE_PIN 32
#define LEFT_OR_RIGHT_PIN 23
#define UP_OR_DOWN_PIN 22

enum Game
{
    Joystick_snake,
    RIPTetris
}

enum inputState
{
    true,
    neutral,
    false
}

#include <LEDMatrix.h>

// Pin configurations for the top LED matrix
int posPintop[] = {33, 38, 41, 36, 19, 13, 18, 15};
int negPintop[] = {37, 17, 16, 34, 14, 35, 39, 40};

// Pin configurations for the bottom LED matrix
int posPinbot[] = {9, 4, 28, 6, 10, 27, 11, 25};
int negPinbot[] = {5, 12, 24, 8, 26, 7, 3, 2};

// Create instances for LED matrices
LEDMatrix<8, 8> LMtop(posPintop, negPintop);
LEDMatrix<8, 8> LMbot(posPinbot, negPinbot);

bool cursor = true;
int topLM[8][8] = {{0}};
int botLM[8][8] = {{0}};

Game *game = nullptr;

void Menu()
{
    int icon[2][8][8] =
        {
            // First icon
            {{0, 0, 0, 0, 0, 0, 0, 0},
             {0, 1, 1, 1, 1, 1, 1, 0},
             {0, 1, 1, 1, 1, 1, 1, 0},
             {0, 0, 0, 1, 1, 0, 0, 0},
             {0, 0, 0, 1, 1, 0, 0, 0},
             {0, 0, 0, 1, 1, 0, 0, 0},
             {0, 0, 0, 1, 1, 0, 0, 0},
             {0, 0, 0, 0, 0, 0, 0, 0}},

            // Second icon
            {{0, 0, 0, 0, 0, 0, 0, 0},
             {0, 0, 1, 1, 1, 1, 0, 0},
             {0, 1, 0, 0, 0, 0, 1, 0},
             {0, 0, 1, 1, 1, 0, 0, 0},
             {0, 0, 0, 0, 1, 1, 0, 0},
             {0, 1, 0, 0, 0, 0, 1, 0},
             {0, 0, 1, 1, 1, 1, 0, 0},
             {0, 0, 0, 0, 0, 0, 0, 0}},
        };

    // Copy icon data into topLM and botLM
    memcpy(topLM, icon[0], sizeof(topLM));
    memcpy(botLM, icon[1], sizeof(botLM));

    // Reference the active menu based on cursor position
    int(&Ref)[8][8] = (cursor) ? topLM : botLM;

    // Add border

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
}

void checkInput()
{
    int Xvalue = analogRead(UP_OR_DOWN_PIN);
    int Yvalue = analogRead(LEFT_OR_RIGHT_PIN);
    int Zvalue = digitalRead(ROTATE_PIN);

    if (Xvalue > 900)
    {
        alterShape(1); // Move Left
    }
    if (Xvalue < 100)
    {
        alterShape(-1); // Move Right
    }
    if (Yvalue > 900)
    {
        alterShape(1); // Move Left
    }
    if (Yvalue < 100)
    {
        alterShape(0); // Move Down
    }

}

void setup()
{
}

void loop()
{
    checkInput();
    if (game == nullptr)
    {
        Menu();
    }
    else
    {
        game.gameLoop();
    }
}
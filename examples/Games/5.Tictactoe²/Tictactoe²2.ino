#include <iostream>
#include <vector>

using namespace std;

// Represents the state of each cell
enum State
{
    Blank,
    X,
    O
};

// Represents a 3x3 grid
class Grid
{
public:
    State board[3][3];
    bool isWon;
    State winner;

    Grid()
    {
        reset();
    }

    // Reset the grid
    void reset()
    {
        isWon = false;
        winner = Blank;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                board[i][j] = Blank;
            }
        }
    }

    // Make a move and check for a win
    bool makeMove(int x, int y, State s)
    {
        if (board[x][y] != Blank)
            return false;

        board[x][y] = s;
        checkWin(x, y, s);
        return true;
    }

    // Check for a win
    void checkWin(int x, int y, State s)
    {
        bool rowWin = true, colWin = true, diagWin = true, antiDiagWin = true;

        for (int i = 0; i < 3; i++)
        {
            if (board[x][i] != s)
                rowWin = false;
            if (board[i][y] != s)
                colWin = false;
            if (board[i][i] != s)
                diagWin = false;
            if (board[i][2 - i] != s)
                antiDiagWin = false;
        }

        if (rowWin || colWin || diagWin || antiDiagWin)
        {
            isWon = true;
            winner = s;
        }
    }
};

// Represents the main Tic Tac Toe game
class MegaTicTacToe
{
private:
    Grid majorGrid[3][3];
    int currentMajorX, currentMajorY;
    bool firstMove;
    State currentPlayer;

public:
    MegaTicTacToe()
    {
        reset();
    }

    // Reset the game
    void reset()
    {
        firstMove = true;
        currentPlayer = X;
        currentMajorX = -1;
        currentMajorY = -1;

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                majorGrid[i][j].reset();
            }
        }
    }

    // Play a turn
    bool playTurn(int majorX, int majorY, int minorX, int minorY)
    {
        // Validate move
        if (!firstMove && (majorX != currentMajorX || majorY != currentMajorY))
        {
            cout << "Invalid move: Must play in the correct major cell!" << endl;
            return false;
        }

        // Validate major grid
        if (majorGrid[majorX][majorY].isWon)
        {
            cout << "Invalid move: Major grid already won!" << endl;
            return false;
        }

        // Play move
        if (!majorGrid[majorX][majorY].makeMove(minorX, minorY, currentPlayer))
        {
            cout << "Invalid move: Cell already occupied!" << endl;
            return false;
        }

        // Update game state
        firstMove = false;
        currentMajorX = minorX;
        currentMajorY = minorY;

        // Check for overall game win
        checkOverallWin();

        // Switch players
        currentPlayer = (currentPlayer == X) ? O : X;
        return true;
    }

    // Check for overall game win
    void checkOverallWin()
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (majorGrid[i][j].isWon)
                {
                    majorGrid[i][j].winner = currentPlayer;
                }
            }
        }
    }
};

// Example usage in Arduino setup/loop
void setup()
{
    MegaTicTacToe game;

    // Simulate gameplay
    game.playTurn(0, 0, 1, 1); // Player X plays in major(0,0), minor(1,1)
    game.playTurn(1, 1, 2, 2); // Player O plays in major(1,1), minor(2,2)
    // Continue playing...
}

void loop()
{
    // Your Arduino loop logic
}

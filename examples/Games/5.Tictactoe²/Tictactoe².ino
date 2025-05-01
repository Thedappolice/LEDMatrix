enum state
{
    blank,
    X,
    O
};

class Grid
{
public:
    state board[3][3];
    bool isWon;
    state winner;

    Grid()
    {
        reset();
    }

    void reset()
    {
        isWon = false;
        winner = blank;
        memset(board, blank, sizeof(board));
    }

    bool makeMove(int x, int y, state s)
    {
        if (board[y][x] != blank)
            return false;

        board[y][x] = s;
        checkWin(x, y, s);
        return true;
    }

    void checkWin(int x, int y, state s)
    {
        bool rowWin = 1;
        bool colWin = 1;
        bool diagWin = 1;
        bool antiDiagWin = 1;

        for (int i = 0; i < 3, i++)
        {
            if (board[y][i] != s)
                rowWin = false;
            if (board[i][x] != s)
                colWin = false;
            if (board[i][i] != s)
                diagWin = false;
            if (board[i][2-i] != s)
                antiDiagWin = false;
        }

        if (rowWin || colWin || diagWin || antiDiagWin)
        {
            isWon = true;
            winner = s;
        }
    }
};

class megaGrid
{
private:
    Grid majorGrid[3][3];
    int currentMajorX;
    int currentMajorY;
    bool firstMove;
    state currentPlayer;

public:
    megaGrid()
    {
        reset();
    }

    void reset()
    {
        firstMove = 1;
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

void setup()
{
}

void loop()
{
}

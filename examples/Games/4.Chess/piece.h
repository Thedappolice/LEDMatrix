#include <LEDMatrix.h>

// Moveset definitions using named constants for clarity
const uint8_t UP_ONLY = 0x00;
const uint8_t FOUR_DIR_CARDINAL = 0x01;
const uint8_t KNIGHT_L = 0x02;
const uint8_t FOUR_DIR_DIAGONAL = 0x03;
const uint8_t EIGHT_DIR = 0x04;

// Structure to represent each chess piece
struct piece
{
    bool colour;     // 0 for black, 1 for white
    uint8_t moveset; // Movement capability
    bool repeatMove; // Can the piece move multiple squares in a direction?
    bool special;    // Special move (like pawn's first double move)
    bool isAlive;    // Is the piece still in the game?
    char coords[2];  // Board position (e.g., "A1", "B2")
    char Notation;   // Piece notation (R, N, B, Q, K, P)

    // Constructor to initialize all properties
    piece(bool c, uint8_t m, bool r, bool s, bool alive, const char *position, char n)
        : colour(c), moveset(m), repeatMove(r), special(s), isAlive(alive), Notation(n)
    {
        coords[0] = position[0];
        coords[1] = position[1];
    }
};

// Initialize the array of pieces
piece Allpeices[32] = {
    // White pieces
    R piece(1, FOUR_DIR_CARDINAL, 1, 1, 1, "A1", 'R'), // White rook
    N piece(1, KNIGHT_L,          0, 0, 1, "B1", 'N'),          // White knight
    B piece(1, FOUR_DIR_DIAGONAL, 1, 0, 1, "C1", 'B'), // White bishop
    Q piece(1, EIGHT_DIR,         1, 0, 1, "D1", 'Q'),         // White queen
    K piece(1, EIGHT_DIR,         0, 1, 1, "E1", 'K'),         // White king
    B piece(1, FOUR_DIR_DIAGONAL, 1, 0, 1, "F1", 'B'), // White bishop
    N piece(1, KNIGHT_L,          0, 0, 1, "G1", 'N'),          // White knight
    R piece(1, FOUR_DIR_CARDINAL, 1, 1, 1, "H1", 'R'), // White rook

    // White pawns
    A piece(1, UP_ONLY, 0, 1, 1, "A2"), // White pawn A2
    B piece(1, UP_ONLY, 0, 1, 1, "B2"), // White pawn B2
    C piece(1, UP_ONLY, 0, 1, 1, "C2"), // White pawn C2
    D piece(1, UP_ONLY, 0, 1, 1, "D2"), // White pawn D2
    E piece(1, UP_ONLY, 0, 1, 1, "E2"), // White pawn E2
    F piece(1, UP_ONLY, 0, 1, 1, "F2"), // White pawn F2
    G piece(1, UP_ONLY, 0, 1, 1, "G2"), // White pawn G2
    H piece(1, UP_ONLY, 0, 1, 1, "H2"), // White pawn H2

    // Black pieces
    r piece(0, FOUR_DIR_CARDINAL, 1, 1, 1, "A8"), // Black rook
    n piece(0, KNIGHT_L,          0, 0, 1, "B8"),          // Black knight
    b piece(0, FOUR_DIR_DIAGONAL, 1, 0, 1, "C8"), // Black bishop
    q piece(0, EIGHT_DIR,         1, 0, 1, "D8"),         // Black queen
    k piece(0, EIGHT_DIR,         0, 1, 1, "E8"),         // Black king
    b piece(0, FOUR_DIR_DIAGONAL, 1, 0, 1, "F8"), // Black bishop
    n piece(0, KNIGHT_L,          0, 0, 1, "G8", 'n'),          // Black knight
    r piece(0, FOUR_DIR_CARDINAL, 1, 1, 1, "H8"), // Black rook

    // Black pawns
    a piece(0, UP_ONLY, 0, 1, 1, "A7", 'a'), // Black pawn A7
    b piece(0, UP_ONLY, 0, 1, 1, "B7", 'b'), // Black pawn B7
    c piece(0, UP_ONLY, 0, 1, 1, "C7", 'c'), // Black pawn C7
    d piece(0, UP_ONLY, 0, 1, 1, "D7", 'd'), // Black pawn D7
    e piece(0, UP_ONLY, 0, 1, 1, "E7", 'e'), // Black pawn E7
    f piece(0, UP_ONLY, 0, 1, 1, "F7", 'f'), // Black pawn F7
    g piece(0, UP_ONLY, 0, 1, 1, "G7", 'g'), // Black pawn G7
    h piece(0, UP_ONLY, 0, 1, 1, "H7", 'h')  // Black pawn H7
};

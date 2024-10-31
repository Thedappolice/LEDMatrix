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
    piece(1, FOUR_DIR_CARDINAL, 1, 1, 1, "A1", 'R'), // White rook
    piece(1, KNIGHT_L, 0, 0, 1, "B1", 'N'),          // White knight
    piece(1, FOUR_DIR_DIAGONAL, 1, 0, 1, "C1", 'B'), // White bishop
    piece(1, EIGHT_DIR, 1, 0, 1, "D1", 'Q'),         // White queen
    piece(1, EIGHT_DIR, 0, 1, 1, "E1", 'K'),         // White king
    piece(1, FOUR_DIR_DIAGONAL, 1, 0, 1, "F1", 'B'), // White bishop
    piece(1, KNIGHT_L, 0, 0, 1, "G1", 'N'),          // White knight
    piece(1, FOUR_DIR_CARDINAL, 1, 1, 1, "H1", 'R'), // White rook

    // White pawns
    piece(1, UP_ONLY, 0, 1, 1, "A2", 'A'), // White pawn A2
    piece(1, UP_ONLY, 0, 1, 1, "B2", 'B'), // White pawn B2
    piece(1, UP_ONLY, 0, 1, 1, "C2", 'C'), // White pawn C2
    piece(1, UP_ONLY, 0, 1, 1, "D2", 'D'), // White pawn D2
    piece(1, UP_ONLY, 0, 1, 1, "E2", 'E'), // White pawn E2
    piece(1, UP_ONLY, 0, 1, 1, "F2", 'F'), // White pawn F2
    piece(1, UP_ONLY, 0, 1, 1, "G2", 'G'), // White pawn G2
    piece(1, UP_ONLY, 0, 1, 1, "H2", 'H'), // White pawn H2

    // Black pieces
    piece(0, FOUR_DIR_CARDINAL, 1, 1, 1, "A8", 'r'), // Black rook
    piece(0, KNIGHT_L, 0, 0, 1, "B8", 'n'),          // Black knight
    piece(0, FOUR_DIR_DIAGONAL, 1, 0, 1, "C8", 'b'), // Black bishop
    piece(0, EIGHT_DIR, 1, 0, 1, "D8", 'q'),         // Black queen
    piece(0, EIGHT_DIR, 0, 1, 1, "E8", 'k'),         // Black king
    piece(0, FOUR_DIR_DIAGONAL, 1, 0, 1, "F8", 'b'), // Black bishop
    piece(0, KNIGHT_L, 0, 0, 1, "G8", 'n'),          // Black knight
    piece(0, FOUR_DIR_CARDINAL, 1, 1, 1, "H8", 'r'), // Black rook

    // Black pawns
    piece(0, UP_ONLY, 0, 1, 1, "A7", 'a'), // Black pawn A7
    piece(0, UP_ONLY, 0, 1, 1, "B7", 'b'), // Black pawn B7
    piece(0, UP_ONLY, 0, 1, 1, "C7", 'c'), // Black pawn C7
    piece(0, UP_ONLY, 0, 1, 1, "D7", 'd'), // Black pawn D7
    piece(0, UP_ONLY, 0, 1, 1, "E7", 'e'), // Black pawn E7
    piece(0, UP_ONLY, 0, 1, 1, "F7", 'f'), // Black pawn F7
    piece(0, UP_ONLY, 0, 1, 1, "G7", 'g'), // Black pawn G7
    piece(0, UP_ONLY, 0, 1, 1, "H7", 'h')  // Black pawn H7
};

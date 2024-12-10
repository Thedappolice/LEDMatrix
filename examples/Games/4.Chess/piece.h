#include <LEDMatrix.h>

// Moveset definitions using named constants for clarity
const uint8_t UP_ONLY = 0x00;
const uint8_t FOUR_DIR_CARDINAL = 0x01;
const uint8_t KNIGHT_L = 0x02;
const uint8_t FOUR_DIR_DIAGONAL = 0x03;
const uint8_t EIGHT_DIR = 0x04;fsfefsesss

// Structure to represent each chess piece
struct PieceType {
    uint8_t moveset;   // Movement capability
    bool repeatMove;   // Can the piece move multiple squares in a direction?
    char notation;     // Piece notation (R, N, B, Q, K, P)
    bool specialMove;  // Indicates if the piece has a special move
};

// Predefined types for all chess pieces
const PieceType Pawn    = {UP_ONLY,           false, 'P', true};
const PieceType Rook    = {FOUR_DIR_CARDINAL, true, 'R', false};
const PieceType Knight  = {KNIGHT_L,          false, 'N', false};
const PieceType Bishop  = {FOUR_DIR_DIAGONAL, true, 'B', false};
const PieceType Queen   = {EIGHT_DIR,         true, 'Q', false};
const PieceType King    = {EIGHT_DIR,         false, 'K', true};

// Structure to represent each individual piece on the board
struct Piece {
    bool colour;       // 0 for black, 1 for white
    bool isAlive;      // Is the piece still in the game?
    char coords[2];    // Board position (e.g., "A1", "B2")
    const PieceType* type; // Pointer to the type definition

    // Constructor to initialize all properties
    Piece(bool c, bool alive, const char* position, const PieceType* t)
        : colour(c), isAlive(alive), type(t) {
        coords[0] = position[0];
        coords[1] = position[1];
    }
};

// Initialize the array of pieces
Piece AllPieces[32] = {
    // White pieces
    Piece(1, true, "A1", &Rook),
    Piece(1, true, "B1", &Knight),
    Piece(1, true, "C1", &Bishop),
    Piece(1, true, "D1", &Queen),
    Piece(1, true, "E1", &King),
    Piece(1, true, "F1", &Bishop),
    Piece(1, true, "G1", &Knight),
    Piece(1, true, "H1", &Rook),

    // White pawns
    Piece(1, true, "A2", &Pawn),
    Piece(1, true, "B2", &Pawn),
    Piece(1, true, "C2", &Pawn),
    Piece(1, true, "D2", &Pawn),
    Piece(1, true, "E2", &Pawn),
    Piece(1, true, "F2", &Pawn),
    Piece(1, true, "G2", &Pawn),
    Piece(1, true, "H2", &Pawn),

    // Black pieces
    Piece(0, true, "A8", &Rook),
    Piece(0, true, "B8", &Knight),
    Piece(0, true, "C8", &Bishop),
    Piece(0, true, "D8", &Queen),
    Piece(0, true, "E8", &King),
    Piece(0, true, "F8", &Bishop),
    Piece(0, true, "G8", &Knight),
    Piece(0, true, "H8", &Rook),

    // Black pawns
    Piece(0, true, "A7", &Pawn),
    Piece(0, true, "B7", &Pawn),
    Piece(0, true, "C7", &Pawn),
    Piece(0, true, "D7", &Pawn),
    Piece(0, true, "E7", &Pawn),
    Piece(0, true, "F7", &Pawn),
    Piece(0, true, "G7", &Pawn),
    Piece(0, true, "H7", &Pawn)
};

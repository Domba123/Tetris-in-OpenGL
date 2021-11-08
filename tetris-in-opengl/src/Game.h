// Game.h

#include "Square.h"
#include "Piece.h"

// Rows and columns used for the tetris grid
const int ROWS = 20;
const int COLS = 10;

class Game {
public:

	// C-tor
	Game();

	bool canRotate(Piece p);
	bool moveCollision(int dir);
	bool rotationCollision();
	void clearMainGrid();
	void clearNextPieceGrid();
	void genNextPiece();
	void restart();
	void move(int dir);
	void rotateShape(int rot);
	void updateActivePiece();
	void updateNextPieceGrid();
	void fixActivePiece();
	void update();
	void updateActiveAfterCollision();
	void checkLine();
	void clearLine();
	//bool gameOver();
	
	Piece activePiece;
	Piece nextPiece;
	Piece activePieceCopy;

	// Present game grid
	Square mainGrid[ROWS][COLS];

	// Grid for displaying the next piece
	Square nextPieceGrid[5][5];

	// Game data
	bool killed;	
	bool paused;
	bool deleteLines;
	int linesCleared;
	int shapesCount;
	int timer;		
};

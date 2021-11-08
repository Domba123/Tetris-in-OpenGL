// Piece.cpp

#include "Piece.h"

Piece::Piece() {
	Piece(0);
}

//	Create new piece, set its type, color and base rotation
Piece::Piece(int numType) {
	type = numType;
	switch (type) {
		case 0: color(1.0, 0.0, 1.0); break; // square - yellow
		case 1: color(0.0, 1.0, 1.0); break; // line - light blue
		case 2: color(0.7, 1.0, 0.0); break; // T - purple
		case 3: color(0.0, 1.0, 0.0); break; // J - blue
		case 4: color(1.0, 0.0, 0.7); break; // L - orange
		case 5: color(0.0, 0.0, 1.0); break; // S - light green
		case 6: color(1.0, 0.0, 0.0); break; // Z - red
	}
	rotation = 0;
}

//	Get array with information about a rotation of a piece
const int* Piece::rotations() {
	return gamePieces[type][rotation];
}

//	Set piece color
// To-do: Fix so that it matches RBG
void Piece::color(float r, float g, float b) {
	redVal = r;
	greenVal = g;
	blueVal = b;
}

void Piece::rotatePiece(int dir) {
	if(dir > 0) {
		if (rotation == 3)
			rotation = 0;
		else
			rotation += dir;
	}
	else {
		if (rotation == 0)
			rotation = 3;
		else
			rotation += dir;
	}
}

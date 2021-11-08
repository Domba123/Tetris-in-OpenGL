// Main.cpp

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <time.h>
#include "glut.h"
#include "Game.h"

using namespace std;
const int BLOCKSIZE = 40;
const int VPWIDTH = COLS * BLOCKSIZE;
const int VPHEIGHT = ROWS * BLOCKSIZE;

Game game;
GLvoid* font_style = GLUT_BITMAP_TIMES_ROMAN_24;

int score_color = 1;

// Displaying text
void BitmapText(char *str, int wcx, int wcy)
{
	glRasterPos2i(wcx, wcy);
	for (int i=0; str[i] != '\0'; i++) {
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[i]);		
	}
}

// Get random number on the interval [0, 1]
double random()
{ 
	int r = rand();
	return double(r) / RAND_MAX;
}

void timer(int id)
{
	if (game.killed) {	// Game over

		game.paused = true;
		game.timer = 500;
		game.clearMainGrid();
		game.clearNextPieceGrid();
		glutPostRedisplay();

	} else if (!game.paused) {	// Game is ongoing
		game.update();		
		if (game.killed) {
			glutTimerFunc(10, timer, 1);
		} else {
			glutPostRedisplay();
			(game.timer > 100) ? game.timer -= 1 : game.timer = 100; // Make it progressively harder as time goes on (by making the blocks fall faster)
			glutTimerFunc(game.timer, timer, 0);
		}
	}
}

// Game controls
void keyboard(unsigned char key, int x, int y)
{
	if (game.paused && game.killed) {
		if (key == 13) { // 13 == Enter -- Start a new game
			game.killed = false;
			game.restart();
			glutTimerFunc(game.timer, timer, 0);
		}
	}
	else {
		if (key == 'p' || key == 27) { // 27 == Escape -- Pause
			game.paused = !game.paused;
			if (!game.paused)
				glutTimerFunc(game.timer, timer, 0);
		}
		else if (!game.paused && !game.killed && key == ' ' || key == 'x') { // ' ' == Space -- Make the tetromino fast fall
			game.update();
		}
		else if (!game.paused && !game.killed && key == 'r') { // Restart game
				game.restart();
				
			
		}
	}

	glutPostRedisplay();
}

// Movement controls
void special(int key, int x, int y)
{
	if (!game.paused && !game.killed) {
		if (key == GLUT_KEY_LEFT) {
			game.move(-1);
			glutPostRedisplay();
		} else if (key == GLUT_KEY_RIGHT) {
			game.move(1);
			glutPostRedisplay();
		} else if (key == GLUT_KEY_UP) {
			game.rotateShape(1);
			glutPostRedisplay();
		} else if (key == GLUT_KEY_DOWN) {
			game.rotateShape(-1);
			glutPostRedisplay();
		}
	}
}

// Main drawing function - displays everything in the window
void display(void)
{
	const int buffer = 100;
	char msg[buffer + 1];

	glClearColor(0.3f, 0.3f, 0.3f, 0.52);
	glClear(GL_COLOR_BUFFER_BIT);

	// Grid
	glViewport(0, 0, ((VPWIDTH * 2) / 3) * 2, VPHEIGHT); // Make the main grid take the biggest portion of the window
	glMatrixMode(GL_PROJECTION);

	if (!game.paused) {	// Game is ongoing - move the tetromino(es)

		glLoadIdentity();
		gluOrtho2D(0, COLS, ROWS, 0);

		for (int row = 0; row < ROWS; row++) {
			for (int col = 0; col < COLS; col++) {
				Square& square = game.mainGrid[row][col];
				if (square.isFilled) {
					
					glColor3f(square.red, square.green, square.blue);

					glRectd(col, row, col+.96, row+.96);					
					
					float offset = .01;
					glColor3ub(0, 0, 0); // Black
					glLineWidth(4);

					// Draw Tetromino border
					glBegin(GL_LINE_LOOP);
					glVertex2f(			col + offset,			row + offset);
					glVertex2f(	(col + .97) - offset,			row + offset);
					glVertex2f(	(col + .97) - offset,	(row + .97) - offset);
					glVertex2f(			col + offset,	(row + .97) - offset);
					glEnd();


				}
				else { // Fill in the background
					glColor3f(0.7, 0.7, 0.7);
					glRectd(col, row, col + 1, row + 1);
				}
			}
		}
	}
	else {

		glLoadIdentity();
		gluOrtho2D(0, VPWIDTH, 0, VPHEIGHT);

		if (game.paused && !game.killed) {		// Paused -> display pause menu
			glColor3f(1, 1, 1); // White
			sprintf_s(msg, buffer, "GAME PAUSED");
			BitmapText(msg, 140, VPHEIGHT / 2);
		}
		if (game.paused && game.killed) {		// Game over -> display game over menu
			glColor3f(1, 1, 1); // White
			sprintf_s(msg, buffer, "GAME OVER");
			BitmapText(msg,  155, VPHEIGHT/2 + 50);
			sprintf_s(msg, buffer, "SCORE: %d", game.linesCleared);
			BitmapText(msg,  155, VPHEIGHT/2);
			sprintf_s(msg, buffer, "Press [ENTER] to restart ...");
			BitmapText(msg, 100, VPHEIGHT/2 - 100);
		}
	}

	// Vertical divider
	glViewport(((VPWIDTH * 2) / 3) * 2, 0, VPWIDTH, VPHEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, ((VPWIDTH * 2) / 3) * 2, 0, VPHEIGHT);

	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0);
	glVertex2d(1, 0);
	glVertex2d(1, glutGet(GLUT_WINDOW_HEIGHT));
	glEnd();

	// UI Text
	glLoadIdentity();
	gluOrtho2D(0, VPWIDTH, 0, VPHEIGHT);

	glColor3f(1, 1, 1);
	sprintf_s(msg, buffer, "Lines cleared: %d", game.linesCleared);
	BitmapText(msg, 15, 100);

	sprintf_s(msg, buffer, "Tetrominoes encountered: %d", game.shapesCount);
	BitmapText(msg, 15, 50);
	sprintf_s(msg, buffer, "Next shape:");
	BitmapText(msg, 15, VPHEIGHT - 50);

	sprintf_s(msg, buffer, "CONTROLS");
	BitmapText(msg, 15, VPHEIGHT/2 + 50);
	sprintf_s(msg, buffer, "Move - Arrows left & right");
	BitmapText(msg, 15, VPHEIGHT/2 + 30);
	sprintf_s(msg, buffer, "Rotate - Arrows up & down");
	BitmapText(msg, 15, VPHEIGHT/2 + 10);
	sprintf_s(msg, buffer, "Fast fall - X or Space");
	BitmapText(msg, 15, VPHEIGHT/2 - 10);
	sprintf_s(msg, buffer, "Restart - R");
	BitmapText(msg, 15, VPHEIGHT/2 - 30);

	glLineWidth(2);
	glBegin(GL_LINE_LOOP);
	switch (score_color) {
		case 1:		glColor3f(0.7, 0.7, 0.0); score_color += 1; break;
		case 2:		glColor3f(0.0, 0.7, 0.2); score_color += 1; break;
		case 3:		glColor3f(1.0, 0.0, 1.0); score_color += 1; break;
		case 4:		glColor3f(0.7, 0.5, 0.0); score_color += 1; break;
		case 5:		glColor3f(0.0, 0.5, 0.7); score_color += 1; break;
		case 6:		glColor3f(0.0, 1.0, 0.0); score_color += 1; break;
		case 7:		glColor3f(1.0, 0.7, 0.0); score_color += 1; break;
		case 8:		glColor3f(0.2, 0.7, 0.2); score_color += 1; break;
		case 9:		glColor3f(0.2, 0.2, 1.0); score_color += 1; break;
		case 10:	glColor3f(1.0, 0.2, 0.2); score_color = 1; break;
		default:	glColor3f(0.7, 0.7, 0.0); break;
	}

	glVertex2d(10, 130);
	glVertex2d(263, 130);
	glVertex2d(263, 30);
	glVertex2d(10, 30);
	glEnd();

	// Vertical divider
	glBegin(GL_LINES);
		glColor3f(1.0, 1.0, 1.0);
		glVertex2d(1,0);
		glVertex2d(1, glutGet(GLUT_WINDOW_HEIGHT));
	glEnd();

	// Next piece grid
	glViewport(((VPWIDTH * 2) / 3) * 2 + 90, -50, VPWIDTH, VPHEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, COLS, ROWS, 0);

	for (int row = 1; row < 5; row++) {
		for (int col = 0; col < 2; col++) {
			Square& square = game.nextPieceGrid[row][col];
			if (square.isFilled) {
				glColor3f(square.red, square.green, square.blue);
				glRectd(col+0.08, row+0.08, col + .96, row + .96);

				float offset = .01;
				glColor3ub(0, 0, 0);
				glLineWidth(4);
				
				// Draw Tetromino border
				glBegin(GL_LINE_LOOP);
				glVertex2f(			col + offset,			row + offset);
				glVertex2f(	(col + .97) - offset,			row + offset);
				glVertex2f(	(col + .97) - offset,	(row + .97) - offset);
				glVertex2f(			col + offset,	(row + .97) - offset);

				glEnd();
			}
			else {
				glColor3f(0.3, 0.3, 0.3);
				glRectd(col, row, col + 1, row + 1);
			}
		}
	}

	glutSwapBuffers();
}

void main(int argc, char *argv[])
{
	srand(time(0));
	glutInit(&argc, argv);


	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);	

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(VPWIDTH*2, VPHEIGHT);

	glutCreateWindow("Tetris OpenGL");

	glutDisplayFunc(display);
	glutSpecialFunc(special);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(game.timer, timer, 0);

	glutMainLoop();
}

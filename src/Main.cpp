// Nguyễn Như Cường - 20200076

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

void BitmapText(char* str, int wcx, int wcy)
{
	glRasterPos2i(wcx, wcy);
	for (int i = 0; str[i] != '\0'; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[i]);
	}
}

// Lấy ramdom 1 số trong khoảng [0, 1]
double random()
{
	int r = rand();
	return double(r) / RAND_MAX;
}

// Bộ hẹn giờ
void timer(int id)
{
	if (game.killed) // Nếu bạn chơi thua
	{

		game.paused = true;
		game.clearMainGrid();
		game.clearNextPieceGrid();
		glutPostRedisplay();
	}
	else if (!game.paused)
	{
		game.update();

		if (game.killed)
		{
			glutTimerFunc(10, timer, 1);
		}
		else
		{
			glutPostRedisplay();
			glutTimerFunc(game.timer, timer, 0);
		}
	}
}

// Xử lý các thao tác: nhấn 'F1' để chơi lại game, nhấn 'F2' để pause game và các phím mũi tên để xoay và di chuyển các khối
void Handler(int key, int x, int y)
{
	if (game.paused && game.killed)
	{
		if (key == GLUT_KEY_F1)		// 13 = "ENTER"
		{
			game.killed = false;
			game.restart();
			glutTimerFunc(game.timer, timer, 0);

			glutPostRedisplay();
		}
	}
	else
	{
		if (key == GLUT_KEY_F2)
		{
			game.paused = !game.paused;

			if (!game.paused)
				glutTimerFunc(game.timer, timer, 0);

			glutPostRedisplay();
		}
		else if (key == GLUT_KEY_LEFT)
		{
			game.move(-1);
			glutPostRedisplay();
		}
		else if (key == GLUT_KEY_RIGHT)
		{
			game.move(1);
			glutPostRedisplay();
		}
		else if (key == GLUT_KEY_UP)
		{
			game.rotateShape(1);
			glutPostRedisplay();
		}
		else if (key == GLUT_KEY_DOWN)
		{
			game.rotateShape(-1);
			glutPostRedisplay();
		}
	}
}

// Hiển thị cửa sổ chơi game
void display(void)
{
	const int N = 100;
	char msg[N + 1];

	glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Grid
	glViewport(0, 0, VPWIDTH + 40, VPHEIGHT);
	glMatrixMode(GL_PROJECTION);

	if (!game.paused) // Nếu trò chơi đang hoạt động, di chuyển các khối
	{
		glLoadIdentity();
		gluOrtho2D(0, COLS, ROWS, 0);

		for (int row = 0; row < ROWS; row++)
		{
			for (int col = 0; col < COLS; col++)
			{
				Square& square = game.mainGrid[row][col];

				if (square.isFilled)
				{
					glColor3f(square.red, square.green, square.blue);
					glRectd(col + 0.1, row + 0.1, col + 0.9, row + 0.9);
				}
				else
				{
					glColor3f(0.2, 0.2, 0.2);
					glRectd(col, row, col + 1, row + 1);
				}
			}
		}
	}
	else 
	{
		glLoadIdentity();
		gluOrtho2D(0, VPWIDTH, 0, VPHEIGHT);

		if (game.paused && !game.killed) // Nếu game đang pause, hiển thị "Game Pause" lên màn hình chính
		{		
			glColor3f(1, 1, 1);
			sprintf_s(msg, N, "GAME PAUSED");
			BitmapText(msg, 140, VPHEIGHT / 2);
		}

		if (game.paused && game.killed) // Nếu bạn chơi thua
		{		
			glColor3f(1, 1, 1);
			sprintf_s(msg, N, "GAME OVER");
			BitmapText(msg, 155, VPHEIGHT / 2 + 50);
			sprintf_s(msg, N, "YOUR SCORE: %d", game.score);
			BitmapText(msg, 140, VPHEIGHT / 2);
			sprintf_s(msg, N, "Press ENTER to restart game...");
			BitmapText(msg, 75, VPHEIGHT / 2 - 150);
		}
	}

	// Vạch dọc chia màn game
	glViewport(VPWIDTH + 40, 0, VPWIDTH, VPHEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, VPWIDTH, 0, VPHEIGHT);

	glBegin(GL_LINES);
	glColor3f(1.0, 0, 0);
	glVertex2d(1, 0);
	glVertex2d(1, glutGet(GLUT_WINDOW_HEIGHT));
	glEnd();

	// Các thông tin ở màn hình phụ
	glLoadIdentity();
	gluOrtho2D(50, VPWIDTH, 50, VPHEIGHT);

	glColor3f(1, 1, 1);
	sprintf_s(msg, N, "NEXT PIECE");
	BitmapText(msg, 170, VPHEIGHT - 50);
	sprintf_s(msg, N, "NGUYEN NHU CUONG - 20200076");
	BitmapText(msg, 100, 450);
	sprintf_s(msg, N, "SCORE %d", game.score);
	BitmapText(msg, 180, 380);
	sprintf_s(msg, N, "DO HOA MAY TINH - MI4382");
	BitmapText(msg, 110, 300);

	// Thông tin "NEXT PIECE"
	glViewport(VPWIDTH + 180, -60, VPWIDTH, VPHEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, COLS, ROWS, 0);

	for (int row = 1; row < 5; row++)
	{
		for (int col = 0; col < 2; col++)
		{
			// Truy cập đối tượng Square tại vị trí hiện tại trong grid của màm phụ
			Square& square = game.nextPieceGrid[row][col];

			if (square.isFilled)
			{
				glColor3f(square.red, square.green, square.blue);
				glRectd(col + 0.1, row + 0.1, col + 0.9, row + 0.9);
			}
			else
			{
				glColor3f(0.2, 0.2, 0.2);
				glRectd(col, row, col + 1, row + 1);
			}
		}
	}

	glutSwapBuffers();
}

void main(int argc, char* argv[])
{
	srand(time(0));
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(VPWIDTH * 2, VPHEIGHT);

	glutCreateWindow("Tetris");

	glutDisplayFunc(display);
	glutSpecialFunc(Handler);
	glutTimerFunc(game.timer, timer, 0);

	glutMainLoop();
}
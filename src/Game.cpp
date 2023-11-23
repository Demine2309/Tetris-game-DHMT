// Nguyễn Như Cường - 20200076

#include <cstdlib>
#include <cstdio>
#include <iostream>

#include "glut.h"
#include "Game.h"

using namespace std;

/*
Called whenever the timer expires, but the game does not end or pause
*/
void Game::update()
{
	// Kiểm tra va chạm với các khối
	if (moveCollision(0)) // Nếu có va chạm
	{					
		if (activePiece.y <= 2) 			// kiểm tra xem có kết thúc game không
			killed = true;
		else								// Nếu game vẫn đang diễn ra
		{								
			updateActiveAfterCollision();	// Cố định khối tại vị trí va chạm
			checkLine();					// Kiểm tra xem có dòng nào đã đầy không
			if (deleteLines)				// Nếu có dòng cần xóa, xóa các dòng đã đầy
				clearLine();				
			genNextPiece();					// Tạo khối mới

			clearNextPieceGrid();
			updateNextPieceGrid();

			updateActivePiece();			// Cập nhật vị trí của khối đang diễn ra trong game
		}
	}
	else									// Nếu không có va chạm, khối mới tiếp tục rơi xuống
	{									
		fixActivePiece();
		activePiece.y++;
		updateActivePiece();
	}
}

// Bắt đầu một trò chơi mới và khởi tạo các phần tử cần thiết
void Game::restart()
{
	clearMainGrid();						// Xóa hết các khối còn lại ở màn chơi trước
	clearNextPieceGrid();					// Xóa "NEXT PIECE" của màn chơi trước
	score = 0;								// Reset lại điểm, SCORE = 0
	killed = false;
	paused = false;
	deleteLines = false;

	// Tạo khối (đầu tiên) ngẫu nhiên cho màn mới
	activePiece = Piece(rand() % numPieces);
	activePiece.x = COLS / 2;
	activePiece.y = 0;
	updateActivePiece();

	// Tạo "NEXT PIECE" cho màn mới
	nextPiece = Piece(rand() % numPieces);
	nextPiece.x = COLS / 2;
	nextPiece.y = 0;
	updateNextPieceGrid();
}


// Cập nhật lại game và hiển thị đúng vị trí của khối đang rơi
void Game::fixActivePiece()
{
	// Xác định dữ liệu của khối hiện tại dựa trên loại và vị trí của nó
	const int* trans = activePiece.getPieceMatrix();

	for (int i = 0; i < 8; i += 2)
	{
		Square& square = mainGrid[activePiece.y + trans[i + 1]][activePiece.x + trans[i]];

		// Đặt các ô hoạt động và không hoạt động
		square.isFilled = false;
		square.isActive = false;
	}
}

// Tạo khối kế tiếp
void Game::genNextPiece()
{
	activePiece = nextPiece;
	nextPiece = Piece(rand() % numPieces);
	nextPiece.x = COLS / 2;
	nextPiece.y = 0;
}


// Di chuyển khối sang trái hoặc phải
void Game::move(int dir)
{
	if (moveCollision(dir))	// Nếu có va chạm với một trong các biên, return.
		return;				
	fixActivePiece();
	activePiece.x += dir;
	updateActivePiece();
}

// Xóa phần màn hình của game
void Game::clearMainGrid()
{
	for (int r = 0; r < ROWS; r++)
	{
		for (int c = 0; c < COLS; c++)
		{
			mainGrid[r][c].isFilled = false;
			mainGrid[r][c].isActive = false;
		}
	}
}

// Xóa và reset lại phần màn phụ
void Game::clearNextPieceGrid()
{
	for (int r = 0; r < 5; r++)
	{
		for (int c = 0; c < 5; c++)
		{
			nextPieceGrid[r][c].isFilled = false;
			nextPieceGrid[r][c].isActive = false;
		}
	}
}

// Cập nhật vị trí của khối trong màn chính sau khi quay
void Game::updateActivePiece()
{
	// Con trỏ đến mảng lưu trữ tất cả các khối
	const int* trans = activePiece.getPieceMatrix();

	// Random 1 khối bất kỳ trong 7 khối của game và đưa nó vào màn chính
	for (int i = 0; i < 8; i += 2)
	{
		Square& square = mainGrid[activePiece.y + trans[i + 1]][activePiece.x + trans[i]];

		square.isFilled = true;
		square.isActive = true;
		square.red = activePiece.redVal;
		square.green = activePiece.blueVal;
		square.blue = activePiece.greenVal;
	}
}

// Cập nhật "NEXT PIECE" trong màn phụ
void Game::updateNextPieceGrid()
{
	// Con trỏ đến mảng lưu trữ các khối
	const int* transNext = nextPiece.getPieceMatrix();

	// Random 1 khối bất kỳ trong 7 khối của game và hiển thị nó trong phần "NEXT PIECE" ở màn phụ
	for (int i = 0; i < 8; i += 2)
	{
		Square& squareNext = nextPieceGrid[nextPiece.y + transNext[i + 1]][nextPiece.x + transNext[i]];

		squareNext.isFilled = true;
		squareNext.isActive = true;
		squareNext.red = nextPiece.redVal;
		squareNext.green = nextPiece.blueVal;
		squareNext.blue = nextPiece.greenVal;
	}
}

// Constructor
Game::Game()
{
	restart();
	timer = 500;
}

// Xoay mảnh hiện tại và kiểm tra xem có thể xoay được hay không
void Game::rotateShape(int dir)
{
	// Tạo một bản sao của mảnh đang hoạt động và kiểm tra xem có thể xoay được hay không
	activePieceCopy = Piece(rand() % numPieces);
	activePieceCopy.x = activePiece.x;
	activePieceCopy.y = activePiece.y;
	activePieceCopy.rotation = activePiece.rotation;
	activePieceCopy.type = activePiece.type;
	activePieceCopy.rotatePiece(dir);

	// Nếu mảnh đang hoạt động có thể xoay, thì xoay và cập nhật hiển thị
	if (canRotate(activePieceCopy))
	{
		fixActivePiece();
		activePiece.rotatePiece(dir);
		updateActivePiece();
	}
}

// Kiểm tra xem một mảnh có thể xoay được hay không
bool Game::canRotate(Piece activeP)
{
	if (rotationCollision())
		return false;
	else
		return true;
}

// Kiểm tra va chạm khi xoay một mảnh
bool Game::rotationCollision()
{
	int x, y;
	const int* trans = activePieceCopy.getPieceMatrix();

	for (int i = 0; i < 8; i += 2)
	{
		x = activePieceCopy.x + trans[i];
		y = activePieceCopy.y + trans[i + 1];

		if (x >= COLS || y >= ROWS || x < 0 || (mainGrid[y][x].isFilled && !mainGrid[y][x].isActive))
			return true;
	}

	return false;
}

// Kiểm tra va chạm khi mảnh đang di chuyển
bool Game::moveCollision(int dir)
{
	int x, y;
	const int* trans = activePiece.getPieceMatrix();

	for (int i = 0; i < 8; i += 2)
	{
		x = activePiece.x + trans[i];
		y = activePiece.y + trans[i + 1];
		if (dir == 0)
			y += 1;
		else
			x += dir;
		if (x >= COLS || y >= ROWS || x < 0 || (mainGrid[y][x].isFilled && !mainGrid[y][x].isActive))
			return true;
	}

	return false;
}

// Cập nhật vị trí của mảnh trong màn chính sau va chạm
void Game::updateActiveAfterCollision()
{
	const int* trans = activePiece.getPieceMatrix();

	for (int i = 0; i < 8; i += 2)
	{
		Square& square = mainGrid[activePiece.y + trans[i + 1]][activePiece.x + trans[i]];
		square.isActive = false;
	}
}

// Kiểm tra các dòng để điền và đánh dấu các dòng đã điền để xóa
void Game::checkLine()
{
	int fullRows = 0;

	for (int r = 0; r < ROWS; r++)
	{
		bool fullRow = false;

		for (int c = 0; c < COLS; c++)
		{
			Square& square = mainGrid[r][c];

			if (square.isFilled)
				fullRow = true;
			else
			{
				fullRow = false;
				break;
			}
		}
		if (fullRow)
		{
			for (int c = 0; c < COLS; c++)
			{
				mainGrid[r][c].toBeDeleted = true;
			}

			deleteLines = true;
			score += 10;
		}
	}
}

// Xóa một dòng đã đầy và di chuyển tất cả các khối xuống một ô 
void Game::clearLine()
{
	for (int r = ROWS - 1; r > 0; r--) // Kiểm tra từng dòng
	{
		int linesDeleted = 0;

		if (mainGrid[r][0].toBeDeleted) // Di chuyển tất cả các dòng xuống một ô
		{
			for (int r2 = r; r2 > 0; r2--)
			{
				for (int c = 0; c < COLS; c++)
				{
					mainGrid[r2][c].isFilled = mainGrid[r2 - 1][c].isFilled;
					mainGrid[r2][c].isActive = mainGrid[r2 - 1][c].isActive;
					mainGrid[r2][c].toBeDeleted = mainGrid[r2 - 1][c].toBeDeleted;
					mainGrid[r2][c].red = mainGrid[r2 - 1][c].red;
					mainGrid[r2][c].green = mainGrid[r2 - 1][c].green;
					mainGrid[r2][c].blue = mainGrid[r2 - 1][c].blue;
				}
			}

			r++;
		}
	}

	deleteLines = false;
}
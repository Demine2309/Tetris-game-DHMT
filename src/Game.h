// Nguyễn Như Cường - 20200076

#include "Square.h"
#include "Piece.h"

// Số hàng và cột của lưới chính
const int ROWS = 20;
const int COLS = 10;

// Game class
class Game 
{
public:

	// Constructor
	Game();

	// Các phương thức chính của Game class
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

	// Các hình thức của khối trong Game
	Piece activePiece;
	Piece nextPiece;
	Piece activePieceCopy;

	// Grid của màn hình chính
	Square mainGrid[ROWS][COLS];

	// Grid của phần "NEXT PIECE" ở màn phụ
	Square nextPieceGrid[5][5];

	// Dữ liệu của Game
	bool killed;
	bool paused;
	bool deleteLines;
	int score;
	int shapesCount;
	int timer;
};
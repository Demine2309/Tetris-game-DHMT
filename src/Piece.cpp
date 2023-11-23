// Nguyễn Như Cường - 20200076

#include "Piece.h"

Piece::Piece()
{
    // Gọi hàm tạo Piece(int) với giá trị mặc định là 0
	Piece(0);
}

// Mảng màu sắc tương ứng với các loại khối
const float pieceColors[][3] = {
    {1.0, 0.75, 0.79},  // Loại 0
    {1.0, 0.75, 0.79},  // Loại 1
    {1.0, 0.75, 0.79},  // Loại 2
    {1.0, 0.75, 0.79},  // Loại 3
    {1.0, 0.75, 0.79},  // Loại 4
    {1.0, 0.75, 0.79},  // Loại 5
    {1.0, 0.75, 0.79}   // Loại 6
};

Piece::Piece(int numType)
{
    type = numType;
    if (type >= 0 && type <= 6)
    {
        setColor(pieceColors[type][0], pieceColors[type][1], pieceColors[type][2]);
    }
    rotation = 0;
}

// Thiết lập màu sắc cho khối
void Piece::setColor(float r, float g, float b)
{
	redVal = r;
	greenVal = g;
	blueVal = b;
}

// Xoay khối theo hướng chỉ định
void Piece::rotatePiece(int dir)
{
    if (dir > 0)
    {
        // Xoay theo chiều kim đồng hồ
        rotation = (rotation + 1) % 4;
    }
    else
    {
        // Xoay ngược chiều kim đồng hồ
        rotation = (rotation + 3) % 4;
    }
}

// Lấy ma trận biểu diễn cho khối dựa trên độ xoay hiện tại
const int* Piece::getPieceMatrix()
{
    return gamePieces[type][rotation];
}
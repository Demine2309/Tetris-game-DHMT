// Nguyễn Như Cường - 20200076

// Các hằng số cơ bản trong game
const int numPieces = 7;			// Số lượng các khối khác nhau
const int numRotations = 4;			// Số lượt xoay cho mỗi khối
const int numSpaces = 8;			// Sức chứa bộ nhớ để lưu thông tin về mỗi khối

// Lớp đối tượng Piece (khối)
class Piece 
{
public:

	// Constructors
	Piece();
	Piece(int newPiece);

	// Tọa độ của khối (tính từ góc trên bên trái của khối, ngay cả khi góc này trống)
	int x;
	int y;

	// Giá trị màu của khối
	float redVal, greenVal, blueVal;

	// Loại và lượt xoay của khối
	int type;
	int rotation;

	// Xoay khối
	void rotatePiece(int dir);

	// Đặt màu sắc cho khối theo định dạng RGB
	void setColor(float r, float g, float b);

	// Các giá trị để xoay khối
	const int* getPieceMatrix();
};

// Tất cả các khối với từng lựa chọn xoay của chúng trong lưới theo định dạng {x0, y0, x1, y1, x2, y2, x3, y3}
const int gamePieces[numPieces][numRotations][numSpaces] =
{
	// Khối vuông
	{
		{0, 0, 1, 0, 0, 1, 1, 1},	
		{0, 0, 1, 0, 0, 1, 1, 1},
		{0, 0, 1, 0, 0, 1, 1, 1},
		{0, 0, 1, 0, 0, 1, 1, 1},
	},

	// Khối đường thẳng
	{
		{0, 0, 0, 1, 0, 2, 0, 3},	
		{0, 0, 1, 0, 2, 0, 3, 0},
		{0, 0, 0, 1, 0, 2, 0, 3},
		{0, 0, 1, 0, 2, 0, 3, 0},
	},

	// Khối chữ T
	{
		{0, 0, 0, 1, 1, 1, 0, 2},	
		{1, 0, 0, 1, 1, 1, 2, 1},
		{0, 1, 1, 0, 1, 1, 1, 2},
		{0, 0, 1, 0, 2, 0, 1, 1}
	},

	// Khối chữ L
	{	{0, 0, 1, 0, 0, 1, 0, 2},	
		{0, 0, 0, 1, 1, 1, 2, 1},
		{1, 0, 1, 1, 0, 2, 1, 2},
		{0, 0, 1, 0, 2, 0, 2, 1}
	},

	// Khối chữ L (ngược)
	{	{0, 0, 1, 0, 1, 1, 1, 2},	
		{0, 0, 1, 0, 2, 0, 0, 1},
		{0, 0, 0, 1, 0, 2, 1, 2},
		{2, 0, 0, 1, 1, 1, 2, 1}
	},

	// Khối chữ Z 
	{	{0, 0, 0, 1, 1, 1, 1, 2},	
		{1, 0, 2, 0, 0, 1, 1, 1},
		{0, 0, 0, 1, 1, 1, 1, 2},
		{1, 0, 2, 0, 0, 1, 1, 1}
	},

	// Khối chữ Z (ngược)
	{	{1, 0, 0, 1, 1, 1, 0, 2},	
		{0, 0, 1, 0, 1, 1, 2, 1},
		{1, 0, 0, 1, 1, 1, 0, 2},
		{0, 0, 1, 0, 1, 1, 2, 1}
	}
};
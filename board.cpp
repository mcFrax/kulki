#include <QGraphicsSceneMouseEvent>

#include "board.hpp"
#include "boardimplementation.hpp"
#include "square.hpp"
#include "ball.hpp"

#include "debugtools.hpp"

using namespace std;

//klasa do "udawanych" zamian
//dziala identycznie z Board::BoardInfo, ale zamienia s1 z s2
class SwapPretender
{
	private:
		Square** array;
		uint arrayWidth;
		uint arrayHeight;
		Square* s1;
		Square* s2;
	public:
		SwapPretender(Square** array, uint arrayWidth, uint arrayHeight,
				Square* s1, Square* s2)
			: array(array), arrayWidth(arrayWidth), 
					arrayHeight(arrayHeight), s1(s1), s2(s2)
		{
		}
		BallColor operator () (uint x, uint y) const
		{
			Square* square = array[y*arrayWidth+x];
			if (square == s1)
				square = s2;
			else if (square == s2)
				square = s1;
			return square->ballColor();
		}
		uint width() const
		{
			return arrayWidth;
		}
		uint height() const
		{
			return arrayHeight;
		}
		bool contains(int x, int y) const
		{
			return x >= 0 && x < arrayWidth
				&& y >= 0 && y < arrayHeight;
		}
};

Board* Board::newBoard(const GameSetup& s, QObject * parent)
{
	return new BoardImplementation(s, parent);
}

Board::Board(const GameSetup& s, QObject * parent)
	: QGraphicsScene(0, 0, s.width*Square::xSize,
		s.height*Square::ySize, parent), setup(s), curPlayer(0)
{
}

Player* Board::currentPlayer()
{
	return curPlayer;
}

const Board::GameSetup& Board::gameSetup()
{
	return setup;
}

Board::State Board::getState()
{
	return state;
}

bool Board::inBoard(int x, int y)
{
	return x >= 0 && x < setup.width
		&& y >= 0 && y < setup.height;
}

bool Board::isLegal(Square* s1, Square* s2) const
{
	return legalMoves.find(make_pair(s1, s2)) != legalMoves.end();
}

Board::BoardInfo::BoardInfo(Square** array, uint width, uint height)
	: array(array), arrayWidth(width), arrayHeight(height)
{
}

BallColor Board::BoardInfo::operator () (uint x, uint y) const
{
	return array[y*arrayWidth+x]->ballColor();
}

uint Board::BoardInfo::width() const
{
	return arrayWidth;
}

uint Board::BoardInfo::height() const
{
	return arrayHeight;
}

bool Board::BoardInfo::contains(int x, int y) const
{
	return x >= 0 && x < arrayWidth
		&& y >= 0 && y < arrayHeight;
}

BoardImplementation::BoardImplementation(const GameSetup& s, QObject * parent)
	: Board(s, parent)
{
	BallColor::createTable(setup.colors);
	squares = new Square*[s.width*s.height];
	
	//creating squares
	for (uint iy = 0; iy < s.height; ++iy){
		for (uint ix = 0; ix < s.width; ++ix){
			squares[iy*s.width+ix] = new Square(ix, iy, this);
		}
	}
	
	//setting squares neighbours
	for (uint iy = 0; iy < s.height; ++iy){
		for (uint ix = 0; ix < s.width; ++ix){
			square(ix, iy)->setNeighbours(
					ix == 0 ? 0 : square(ix-1, iy),
					iy == 0 ? 0 : square(ix, iy-1),
					ix == s.width-1  ? 0 : square(ix+1, iy),
					iy == s.height-1 ? 0 : square(ix, iy+1)
					);
		}
	}
	
	//placing balls
	#warning here must go avoiding triplets on start
	#warning possibly by making continous fall till its good
	for (uint iy = 0; iy < s.height; ++iy){
		for (uint ix = 0; ix < s.width; ++ix){
			Ball::getNew(setup, square(ix, iy));
		}
	}
	
	computeLegalMoves();
	state = normal;
}

BoardImplementation::~BoardImplementation()
{
	delete squares;
}

void BoardImplementation::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	switch (state) {
		case playerMove :
			QGraphicsScene::mousePressEvent(event);
			return;
		default:
			event->ignore();
	}
}

void BoardImplementation::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	switch (state) {
		case playerMove :
			QGraphicsScene::mouseMoveEvent(event);
			return;
		default:
			event->ignore();
	}
}

void BoardImplementation::squarePressed(Square* s1, Square* s2)
{
	#warning check if legal?
	Ball* b1 = s1->getBall();
	Ball* b2 = s2->getBall();
	b1->placeOnSquare(s2);
	b2->placeOnSquare(s1);
	
	check();
}

//dzieki template'owi moge uzywac i BoardInfo i SwapPretender,
//bez polimorfizmu i bez copy'iego paste'a
template <class BoardInfo_or_SwapPretender>
uint countSame(uint sX, uint sY, bool xM, bool yM, BallColor bc,
		const BoardInfo_or_SwapPretender& board)
//xMove, yMove definiuja kierunek, nie zwrot
{
	uint res = 0;
	int ix = sX;
	int iy = sY;
	while (board.contains(ix, iy) && board(ix, iy) == bc){
		++res;
		ix += xM;
		iy += yM;
	}
	ix = sX-xM;
	iy = sY-yM;
	while (board.contains(ix, iy) && board(ix, iy) == bc){
		++res;
		ix -= xM;
		iy -= yM;
	}
	return res;
}

template <class BoardInfo_or_SwapPretender>
static bool checkForTriple(uint x, uint y, const Board::GameSetup& setup,
		const BoardInfo_or_SwapPretender& board)
//sprawdza, czy jest jakas trojka (rzad), zawierajacy kulke na (x,y)
{
	for (uint c = 0; c < setup.colors; ++c){
		if (countSame(x, y, 1, 0, c, board) >= setup.rowLength
				|| countSame(x, y, 0, 1, c, board) >= setup.rowLength)
			return 1;
	}
	return 0;
}

void BoardImplementation::computeLegalMoves(const int dx, const int dy)
{
	//wylicza legalne zamiany typu <x,y> z <x+dx, y+dy>
	for ( uint iy = 0; iy < setup.height-dy; ++iy){
		for ( uint ix = 0; ix < setup.width-dx; ++ix){
			//symulacja zamiany, i sprawdzenie, czy cos daje
			SwapPretender pret(squares, setup.width, setup.height, 
					square(ix, iy), square(ix+dx, iy+dy));
			if (checkForTriple(ix, iy, setup, pret)
					|| checkForTriple(ix+dx, iy+dy, setup, pret)){
				legalMoves.insert(make_pair(square(ix, iy), 
						square(ix+dx, iy+dy)));
				legalMoves.insert(make_pair(square(ix+dx, iy+dy), 
						square(ix, iy)));
			}
		}
	}
}

void BoardImplementation::computeLegalMoves()
{
	legalMoves.clear();
	computeLegalMoves(1, 0);
	computeLegalMoves(0, 1);
}

Square* BoardImplementation::square(uint x, uint y)
{
	return squares[y*setup.width+x];
}

void BoardImplementation::check()
{
	state = checking;
	#warning here goes check
	if (0)
		state = falling;
	else if (1)
		state = normal;
	else
		state = locked;
}

#warning some documentation needed here (or there)
void BoardImplementation::setCurrentPlayer(Player* p)
{
	#warning not the best solution:
	if (state != normal)
		return;
	#warning if (curPlayer) ???
	#warning if (!p) ???
	state = playerMove;
	curPlayer = p;
	//legalMoves should be ready here
}
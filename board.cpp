#include <QGraphicsSceneMouseEvent>
#include "board.hpp"
#include "boardimplementation.hpp"
#include "square.hpp"
#include "ball.hpp"

//klasa do "udawanych" zamian
//dziala jak Board::BoardInfo, ale zamienia s1 z s2
class SwapPretender
{
	private:
		Square** array;
		uint arrayWidth;
		Square* s1;
		Square* s2;
	public:
		SwapPretender(Square** array, uint arrayWidth,
				Square* s1, Square* s2)
			: array(array), arrayWidth(arrayWidth), s1(s1), s2(s2)
		{
		}
		BallColor operator () (uint x, uint y)
		{
			Square* square = array[y*arrayWidth+x];
			if (square == s1)
				square = s2;
			else if (square == s2)
				square = s1;
			return square->ballColor();
		}
}

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
	#warning possibly by making fall
	for (uint iy = 0; iy < s.height; ++iy){
		for (uint ix = 0; ix < s.width; ++ix){
			Ball::getNew(setup, square(ix, iy));
		}
	}
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

uint countSame(uint sX, uint sY, bool xM, bool yM, BallColor bc)
//xMove, yMove definiuja kierunek, nie zwrot
{
	uint res = 0;
	int ix = sX;
	int iy = sY;
	while (square(ix, iy)->ballColor() == bc && inBoard(ix, iy)){
		++res;
		ix += xM;
		iy += yM;
	}
	ix = sX-xM;
	iy = sY-yM;
	while (square(ix, iy)->ballColor() == bc && inBoard(ix, iy)){
		++res;
		ix -= xM;
		iy -= yM;
	}
}

static bool checkForTriple(uint x, uint y, Board::GameSetup& setup,
		SwapPretender pret)
{
	for (uint c = 0; c < setup.colors; ++c){
		if (countSame(
	}
}

void BoardImplementation::computeLegalMoves(const int dx, const int dy)
{
	//wylicza legalne zamiany typu <x,y> z <x+dx, y+dy>
	for ( uint iy = 0; iy < setup.height-dx; ++iy){
		for ( uint ix = 0; ix < setup.width-dy; ++ix){
			//symulacja zamiany, i sprawdzenie, czy cos daje
			SwapPretender pret(squares, setup.width, square(ix, iy), 
					square(ix+dx, iy+dy));
			if (checkForTriple(ix, iy, setup, pret)
					|| checkForTriple(ix+dx, iy+dy, setup, pret)){
				legalMoves.insert(make_pair(square(ix, iy), 
						square(ix+dx, iy+dy)));
			}
		}
	}
}

void BoardImplementation::computeLegalMoves()
{
	legalMoves.clear();
	Array2<BallColor> balls(setup.width, setup.height);
	for (uint iy = 0; iy < setup.height; ++iy){
		for (uint ix = 0; ix < setup.width; ++ix){
			balls(ix, iy) = square(ix, iy)->ballColor();
		}
	}
	computeLegalMoves(1, 0, balls);
	computeLegalMoves(0, 1, balls);
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
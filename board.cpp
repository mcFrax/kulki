#include <QGraphicsSceneMouseEvent>
#include "board.hpp"
#include "boardimplementation.hpp"
#include "square.hpp"
#include "ball.hpp"

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

uint BoardImplementation::countSame(uint sX, uint sY, bool xM, bool yM, BallColor bc)
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


void BoardImplementation::computeLegalMoves(int xo, int yo)
{
	for ( uint iy = 0; iy < setup.height-1; ++iy){
		for ( uint ix = 0; ix < setup.width-1; ++ix){
			
		}
	}
}

void BoardImplementation::computeLegalMoves()
{
	legalMoves.clear();
	computeLegalMoves(1,0);
	computeLegalMoves(0,1);
}

Square* BoardImplementation::square(uint x, uint y)
{
	return squares[y*setup.width+x];
}
#include <QGraphicsSceneMouseEvent>
#include "board.hpp"
#include "square.hpp"
#include "ball.hpp"

Board::Board(const GameSetup& s, QObject * parent)
	: QGraphicsScene(0, 0, s.width*Square::xSize,
		s.height*Square::ySize, parent), setup(s), curPlayer(0)
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

Board::~Board()
{
	delete squares;
}

void Board::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	switch (state) {
		case playerMove :
			QGraphicsScene::mousePressEvent(event);
			return;
		default:
			event->ignore();
	}
}

void Board::squarePressed(Square* s1, Square* s2)
{
	Ball* b1 = s1->getBall();
	Ball* b2 = s2->getBall();
	b1->placeOnSquare(s2);
	b2->placeOnSquare(s1);
	
	check();
}

uint Board::countSame(uint startX, uint startY, int xMove, int yMove)
//xMove, yMove definiuja kierunek, nie zwrot
{
	BallColor bc = square(startX, startY).getColor();
	switch (uint(bc)) {
		case BallColor::noneNumber :
			return 0;
		case BallColor::jokerNumber :
			
	}
}


void Board::computeLegalMoves(int xo, int yo)
{
	for ( uint iy = 0; iy < setup.height-1; ++iy){
		for ( uint ix = 0; ix < setup.width-1; ++ix){
			
		}
	}
}

void Board::computeLegalMoves()
{
	legalMoves.clear();
	computeLegalMoves(1,0);
	computeLegalMoves(0,1);
}

Square* Board::square(uint x, uint y)
{
	return squares[y*setup.width+x];
}
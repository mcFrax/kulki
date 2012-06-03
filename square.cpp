#include <QGraphicsSceneHoverEvent>
#include <QPen>

#include "square.hpp"
#include "board.hpp"
#include "ball.hpp"
#include "ballitem.hpp"

#include "settings.hpp"
#include "debugtools.hpp"

Square::Square(uint xPos, uint yPos, Board* b)
	: QGraphicsRectItem(0, 0, size(), size(), b), xPos(xPos), yPos(yPos),
		board(b)
{
	setPen(Qt::NoPen);
	setPos(xPos*size(), yPos*size());
	for (int i = 0; i < 5; ++i)
		neighbours[i] = 0;
}

void Square::setNeighbours(Square* l, Square* t, Square* r, Square* b)
{
	neighbours[0] = l;
	neighbours[1] = t;
	neighbours[2] = r;
	neighbours[3] = b;
	//neighbours[4] = 0;
}

Square* Square::getNeighbour(Square::Side s)
{
	return neighbours[s];
}

Square*const* Square::getNeighbours() const
{
	return neighbours;
}

Ball* Square::getBall()
{
	return ball;
}

Board* Square::getBoard()
{
	return board;
}

BallColor Square::ballColor()
{
	return ball->getColor();
	//~ if (ball)
		//~ return ball->getColor();
	//~ else
		//~ return BallColor::none;
}

QGraphicsItem* Square::item()
{
	return this;
}

QPointF Square::center() const
{
	return pos()+rect().center();
}

int Square::size()
{
	return settings()->value("square/size").toInt();
}

//!Sciaga najblizsza kulke z gory (jezeli taka jest)
bool Square::gravity(bool immediate)
{
	//!\return Czy jakas kulka w efekcie spada
	if (!immediate) return gravity(0);
	if (ball) return 0;
	for (Square* s = this->neighbours[top]; s != 0; s = s->neighbours[top]) {
		if (s->ball) {
			s->ball->placeOnSquare(this);
			return 1;
		}
	}
	return 0;
}

//!Sciaga najblizsza kulke z gory (jezeli taka jest)
bool Square::gravity(int animDelay)
{
	//!\return Czy jakas kulka w efekcie spada
	if (ball) return 0;
	int fall = 1;
	for (Square* s = this->neighbours[top]; s != 0; s = s->neighbours[top]) {
		if (s->ball) {
			s->ball->placeOnSquare(this, fall, animDelay);
			return 1;
		}
		++fall;
	}
	return 0;
}

bool Square::ensureHavingBall(int animDelay)
{
	if (ball)
		return 0;
	Ball::getNew(board->gameSetup(), this, 0, animDelay);
	return 1;
}

bool Square::ensureHavingBall()
{
	if (ball)
		return 0;
	Ball::getNew(board->gameSetup(), this, -1);
	return 1;
}
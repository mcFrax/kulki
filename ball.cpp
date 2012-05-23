#include "ball.hpp"
#include "ballitem.hpp"
#include "balltypes.hpp"
#include "square.hpp"
#include "board.hpp"

Ball::Ball(const BallColor& c)
	: color(c), square(0), state(none), ballItem(0)
{
}

Ball::Ball(const BallColor& c, Square* s, int fall)
	: color(c), square(s), state(fall?falling:normal),
		ballItem(new BallItem(c, s, -fall*Square::ySize))
{
	s->ball = this;
}

void Ball::placeOnSquare(Square* s, int fall)
{
	#warning 
	if (square)
		square->ball = 0;
	if (!ballItem)
		ballItem = new BallItem(color, s, -fall*Square::ySize);
	else
		ballItem->placeOnSquare(s, -fall*Square::ySize);
	state = fall?falling:normal;
	square = s;
	s->ball = this;
}

Ball::~Ball()
{
	if (ballItem)
		delete ballItem;
	if (square)
		square->ball = 0;
}

int Ball::applyPointModificator(const int& points) const
{
	return points;
}

void Ball::update(uint plyNumber)
{
}

//~ BallItem* Ball::getBallItem()
//~ {
	//~ return ballItem;
//~ }

BallColor Ball::getColor() const
{
	return color;
}

void Ball::setColor(const BallColor& col)
{
	color = col;
	ballItem->setBrush(QBrush(col));
}

Ball* Ball::getNew(const Board::GameSetup&)
{
	return new ColorBall(BallColor::random());
}

Ball* Ball::getNew(const Board::GameSetup&, Square* s, int falling)
{
	return new ColorBall(BallColor::random(), s, falling);
}
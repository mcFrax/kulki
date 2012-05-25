#include <cstdlib>

#include "ball.hpp"
#include "ballitem.hpp"
#include "balltypes.hpp"
#include "square.hpp"
#include "board.hpp"

#include "debugtools.hpp"

Ball::Ball(const BallColor& c, Square* s, int fall, int animDelay)
	: color(c), square(s), 
		ballItem(new BallItem(c, s, -fall*Square::ySize, animDelay))
{
	s->ball = this;
}

void Ball::placeOnSquare(Square* s, int fall, int animDelay)
{
	#warning 
	if (square)
		square->ball = 0;
	if (!ballItem)
		ballItem = new BallItem(color, s, -fall*Square::ySize, animDelay);
	else
		ballItem->placeOnSquare(s, -fall*Square::ySize, animDelay);
	square = s;
	if (s)
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

void Ball::newTurnUpdate()
{
}

void Ball::newCheckUpdate()
{
}

void Ball::explode()
{
	if (ballItem){
		ballItem->explode();
		ballItem = 0;
	}
	delete this;
}

void Ball::detach()
{
	placeOnSquare(0);
}

BallColor Ball::getColor() const
{
	return color;
}

QRectF Ball::getRect() const
{
	return ballItem->sceneBoundingRect();
}

void Ball::setColor(const BallColor& col)
{
	color = col;
	ballItem->setBrush(QBrush(col));
}

Ball* Ball::getNew(const Board::GameSetup&, Square* s, int falling, int animDelay)
{
	#warning temporary
	int type = rand()%30;
	switch (type) {
		case 0:
			return new JokerBall(s, falling, animDelay);
		case 1:
			return new SkullBall(BallColor::random(), s, falling, animDelay);
		case 2:
			return new DoubleBall(BallColor::random(), s, falling, animDelay);
		case 3:
			return new CameleonBall(s, falling, animDelay);
		case 4:
			return new HourglassBall(BallColor::random(), s, falling, animDelay);
		default:
			return new ColorBall(BallColor::random(), s, falling, animDelay);
	}
}
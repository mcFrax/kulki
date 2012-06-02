#include <QBrush>

#include <cstdlib>

#include "ball.hpp"
#include "ballitem.hpp"
#include "balltypes.hpp"
#include "square.hpp"
#include "board.hpp"

#include "debugtools.hpp"

int Ball::specialBallTypes = 5;
static const QString specialBallTypeNames[Ball::specialBallTypes] = 
{
	"Joker",
	"Chameleon",
	"Double",
	"Skull",
	"Hourball",
};

Ball::Ball(const BallColor& c, Square* s, int fall, int animDelay)
	: color(c), squareVal(s), 
		ballItem(new BallItem(c, s, -fall*Square::size(), animDelay))
{
	s->ball = this;
}

void Ball::placeOnSquare(Square* s, int fall, int animDelay)
{
	if (squareVal) //may be detached
		squareVal->ball = 0;
	ballItem->placeOnSquare(s, -fall*Square::size(), animDelay);
	squareVal = s;
	if (s)
		s->ball = this;
}

void Ball::placeOnSquare(Square* s, Square* from)
{
	if (squareVal) //may be detached
		squareVal->ball = 0;
	ballItem->placeOnSquare(s, from);
	squareVal = s;
	if (s)
		s->ball = this;
}

Ball::~Ball()
{
	if (ballItem)
		delete ballItem;
	if (squareVal)
		squareVal->ball = 0;
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

void Ball::setColor(const BallColor& col)
{
	color = col;
	ballItem->setBrush(QBrush(col));
}

Square* Ball::square()
{
	return squareVal;
}

//wrappery konstruktorow
template<class BallT>
static Ball* construct1(Square* s, int falling, int animDelay)
{
	return new BallT(s, falling, animDelay);
}

template<class BallT>
static Ball* construct2(Square* s, int falling, int animDelay)
{
	return new BallT(BallColor::random(), s, falling, animDelay);
}

typedef Ball* (*BallConstructor)(Square* s, int falling, int animDelay);

static const int constructors = Ball::specialBallTypes;
static const BallConstructor constructor[constructors] = 
{
	construct1<JokerBall>,
	construct1<ChameleonBall>,
	construct2<DoubleBall>,
	construct2<SkullBall>,
	construct2<HourglassBall>,
};

Ball* Ball::getNew(const Board::GameSetup& setup, Square* s, int falling, int animDelay)
{
	const QList<uint>& bts = setup.ballTypeSettings;
	uint type = rand()%1000;
	int sum = 0;
	for (int i = 0; i < bts.size() && i < constructors; ++i){
		sum += bts[i];
		if (type < bts[i])
			return constructor[i](s, falling, animDelay);
	}
	return construct2<ColorBall>(s, falling, animDelay);
}
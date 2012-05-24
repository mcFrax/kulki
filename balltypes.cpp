#include "balltypes.hpp"


///////////////////////////////////////////////////////////
////////////////////    ColorBall    //////////////////////
///////////////////////////////////////////////////////////

//~ ColorBall::ColorBall(const BallColor& c)
	//~ : Ball(c)
//~ {
//~ }

ColorBall::ColorBall(const BallColor& c, Square* s, int falling, int animDelay)
	: Ball(c, s, falling, animDelay)
{
}

uint ColorBall::getPointValue()
{
	return getColor()+1;
}

///////////////////////////////////////////////////////////
/////////////////////    Joker    /////////////////////////
///////////////////////////////////////////////////////////

//~ JokerBall::JokerBall()
	//~ : Ball(BallColor::joker)
//~ {
//~ }

JokerBall::JokerBall(Square* s, int falling, int animDelay)
	: Ball(BallColor::joker, s, falling, animDelay)
{
}

uint JokerBall::getPointValue()
{
	return 0;
}

///////////////////////////////////////////////////////////
////////////////////    Cameleon    ///////////////////////
///////////////////////////////////////////////////////////

//~ CameleonBall::CameleonBall()
	//~ : ColorBall(BallColor::random())
//~ {
//~ }

CameleonBall::CameleonBall(Square* s, int falling, int animDelay)
	: ColorBall(BallColor::random(), s, falling, animDelay)
{
}

void CameleonBall::update(uint plyNumber)
{
	setColor(BallColor::random());
}

///////////////////////////////////////////////////////////
/////////////////////    Double    ////////////////////////
///////////////////////////////////////////////////////////

//~ DoubleBall::DoubleBall(const BallColor& color)
	//~ : ColorBall(color)
//~ {
//~ }

DoubleBall::DoubleBall(const BallColor& color, Square* s, int falling, int animDelay)
	: ColorBall(color, s, falling, animDelay)
{
}

int DoubleBall::applyPointModificator(const int& points)
{
	return points*2;
}

///////////////////////////////////////////////////////////
/////////////////////    Skull    /////////////////////////
///////////////////////////////////////////////////////////

//~ SkullBall::SkullBall(const BallColor& color)
	//~ : ColorBall(color)
//~ {
//~ }

SkullBall::SkullBall(const BallColor& color, Square* s, int falling, int animDelay)
	: ColorBall(color, s, falling, animDelay)
{
}

uint SkullBall::getPointValue()
{
	return 0;
}

int SkullBall::applyPointModificator(const int& points)
{
	return 0;
}

///////////////////////////////////////////////////////////
////////////////////    Hourglass    //////////////////////
///////////////////////////////////////////////////////////

//~ HourglassBall::HourglassBall(const BallColor& color)
	//~ : ColorBall(color)
//~ {
	//~ points = ColorBall::getPointValue();
//~ }

HourglassBall::HourglassBall(const BallColor& color, Square* s, int falling, int animDelay)
	: ColorBall(color, s, falling, animDelay)
{
	points = ColorBall::getPointValue();
}

void HourglassBall::update(uint plyNumber)
{
	if (points > 0) --points;
}

uint HourglassBall::getPointValue()
{
	return points;
}
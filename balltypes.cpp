#include "balltypes.hpp"
#include "ballitem.hpp"
#include "square.hpp"

#include <QTransform>
#include <QBrush>

#include "debugtools.hpp"

using namespace std;

///////////////////////////////////////////////////////////
////////////////////    ColorBall    //////////////////////
///////////////////////////////////////////////////////////

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

QPixmap* JokerBall::specialPixmap = 0;
QBrush JokerBall::brush;

JokerBall::JokerBall(Square* s, int falling, int animDelay)
	: Ball(BallColor::joker, s, falling, animDelay)
{
	if (!specialPixmap)
		specialPixmap = new QPixmap(":Balls/Joker.png");
	if (brush.style() == Qt::NoBrush){
		QPixmap brushPixmap(QPixmap(":Balls/JokerBrush.png").
			scaledToWidth(Square::size()+40, Qt::SmoothTransformation));
		brush = QBrush(brushPixmap);
		brush.setTransform(QTransform::fromTranslate(-20, -20));
	}
	ballItem->setBrush(brush);
	ballItem->setSpecialPixmap(*specialPixmap, 0.6);
}

uint JokerBall::getPointValue()
{
	return 0;
}

///////////////////////////////////////////////////////////
////////////////////    Cameleon    ///////////////////////
///////////////////////////////////////////////////////////

QPixmap* CameleonBall::specialPixmap = 0;

CameleonBall::CameleonBall(Square* s, int falling, int animDelay)
	: ColorBall(BallColor::random(), s, falling, animDelay)
{
	if (!specialPixmap)
		specialPixmap = new QPixmap(":Balls/Cameleon.png");
	ballItem->setSpecialPixmap(*specialPixmap, 0.6);
}

void CameleonBall::newCheckUpdate()
{
	setColor(BallColor::random());
}

///////////////////////////////////////////////////////////
/////////////////////    Double    ////////////////////////
///////////////////////////////////////////////////////////

QPixmap* DoubleBall::specialPixmap = 0;

DoubleBall::DoubleBall(const BallColor& color, Square* s, int falling, int animDelay)
	: ColorBall(color, s, falling, animDelay)
{
	if (!specialPixmap)
		specialPixmap = new QPixmap(":Balls/Double.png");
	ballItem->setSpecialPixmap(*specialPixmap, 0.6);
}

int DoubleBall::applyPointModificator(const int& points) const
{
	return points*2;
}

///////////////////////////////////////////////////////////
/////////////////////    Skull    /////////////////////////
///////////////////////////////////////////////////////////

QPixmap* SkullBall::specialPixmap = 0;

SkullBall::SkullBall(const BallColor& color, Square* s, int falling, int animDelay)
	: ColorBall(color, s, falling, animDelay)
{
	if (!specialPixmap)
		specialPixmap = new QPixmap(":Balls/Skull.png");
	ballItem->setSpecialPixmap(*specialPixmap, 0.6);
}

uint SkullBall::getPointValue()
{
	return 0;
}

int SkullBall::applyPointModificator(const int& points) const
{
	return 0;
}

///////////////////////////////////////////////////////////
////////////////////    Hourglass    //////////////////////
///////////////////////////////////////////////////////////

QPixmap* HourglassBall::specialPixmap = 0;

HourglassBall::HourglassBall(const BallColor& color, Square* s, int falling, int animDelay)
	: ColorBall(color, s, falling, animDelay)
{
	points = ColorBall::getPointValue();
	if (!specialPixmap)
		specialPixmap = new QPixmap(":Balls/Hourglass.png");
	ballItem->setSpecialPixmap(*specialPixmap, 0.6);
}

void HourglassBall::newTurnUpdate()
{
	if (points > 0) --points;
}

uint HourglassBall::getPointValue()
{
	return points;
}
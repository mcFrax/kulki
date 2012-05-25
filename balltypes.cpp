#include "balltypes.hpp"
#include "ballitem.hpp"
#include "square.hpp"

#include <QTransform>

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
		specialPixmap = new QPixmap("../jester.gif");
	if (brush.style() == Qt::NoBrush){
		QPixmap brushPixmap(QPixmap("../swirl.png").
			scaledToWidth(Square::xSize+40, Qt::SmoothTransformation));
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
		//~ specialPixmap = new QPixmap("../rainbow-partial-moons_Clipart_svg_File.svg");
		specialPixmap = new QPixmap("../rainbow.png");
	ballItem->setSpecialPixmap(*specialPixmap, 0.6);
}

void CameleonBall::newCheckUpdate()
{
	set<BallColor> fb;
	for (int i = 0; i < 4; ++i){
		if (square->getNeighbours()[i]){
			BallColor c = square->getNeighbours()[i]->ballColor();
			if (c.isNormal());
				fb.insert(c);
		}
	}
	setColor(BallColor::random(fb));
	//jezeli 2 kameleony beda obok siebie, to i tak beda zmieniac kolor
	//kolejno, nie jednoczesnie, wiec nie ma problemu
	//jedyna glupia sytuacja, to kiedy obok bedzie joker - wtedy nic nie pomoze
}

///////////////////////////////////////////////////////////
/////////////////////    Double    ////////////////////////
///////////////////////////////////////////////////////////

QPixmap* DoubleBall::specialPixmap = 0;

DoubleBall::DoubleBall(const BallColor& color, Square* s, int falling, int animDelay)
	: ColorBall(color, s, falling, animDelay)
{
	if (!specialPixmap)
		specialPixmap = new QPixmap("../Star.png");
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
		specialPixmap = new QPixmap("../Skull.png");
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
		specialPixmap = new QPixmap("../Hourglass.png");
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
#include "ballcolor.hpp"

#include <cstdlib>

#include <QColor>

using namespace std;

uint BallColor::colors = 0;
const uint BallColor::noneNumber = 0xFFFFFFFF;
const uint BallColor::jokerNumber = 0xFFFFFFFE;
const BallColor BallColor::none = 0xFFFFFFFF;
const BallColor BallColor::joker = 0xFFFFFFFE;
QColor* BallColor::table = 0;

BallColor::BallColor(uint color)
	: colorNumber(color)
{
}

bool BallColor::operator == (const BallColor& c) const
{
	return c.colorNumber != noneNumber &&
			this->colorNumber != noneNumber
			&&
			(c.colorNumber == this->colorNumber
			|| c.colorNumber == jokerNumber
			|| this->colorNumber == jokerNumber);
};

bool BallColor::operator != (const BallColor& color) const
{
	return !(*this == color);
};

bool BallColor::operator < (const BallColor& c) const
{
	return colorNumber < c.colorNumber;
}

BallColor::operator uint() const
{
	return colorNumber;
}

BallColor::operator QColor() const
{
	switch (colorNumber) {
		case noneNumber:
		case jokerNumber:
			return Qt::transparent;
		default:
			return table[colorNumber];
	}
}

bool BallColor::isNormal() const
{
	return colorNumber < 0xFFFFFFF0;
}

void BallColor::createTable(uint cols)
{
	colors = cols;
	#warning ugly & fails for colors > 19 (20th color is transparent)
	#warning and expanding should be possible
	if (table) delete [] table;
	table = new QColor[colors];
	for (uint i = 0; i < colors; ++i)
		#warning disgusting
		table[i] = QColor((Qt::GlobalColor)(i+7));
}

BallColor BallColor::random()
{
	return rand()%colors;
}

//!Losuje kolor nie nalezacy do zbioru forbidden (nieuzywana)
BallColor BallColor::random(const std::set<BallColor>& fb)
{
	BallColor res;
	//losuje, az znajde dobry. potencjalnie moze sie zawiesic.
	while(fb.find(res = random()) != fb.end());
	return res;
}

uint BallColor::availableColors()
{
	return colors;
}
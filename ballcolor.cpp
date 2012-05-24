#include "ballcolor.hpp"

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

BallColor::operator uint() const
{
	return colorNumber;
}

BallColor::operator QColor() const
{
	switch (colorNumber) {
		case noneNumber:
		case jokerNumber:
			#warning temporary
			return Qt::transparent;
		default:
			return table[colorNumber];
	}
}

void BallColor::createTable(uint cols)
{
	colors = cols;
	#warning ugly & fails for colors > 19 (20th color is transparent)
	#warning and expanding should be possible
	if (table) delete table;
	table = new QColor[colors];
	for (uint i = 0; i < colors; ++i)
		#warning disgusting
		table[i] = QColor((Qt::GlobalColor)(i+7));
}

#include <cstdlib>
#include <ctime>
#include <iostream>

bool sra = 0;

BallColor BallColor::random()
{
	#warning very temporary
	if (!sra)
		srand(time(0));
	sra = 1;
	//~ std::cerr << rand()%colors  << std::endl;
	return rand()%colors;
}

uint BallColor::availableColors()
{
	return colors;
}
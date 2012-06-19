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
	const static uint predefinedColors = 20;
	static const QColor colortable[predefinedColors] = {
		QColor(Qt::red),
		QColor(Qt::green),
		QColor(Qt::blue),
		QColor("orange"),
		QColor(Qt::magenta),
		QColor(Qt::darkRed),
		QColor(Qt::darkGreen),
		QColor(Qt::darkBlue),
		QColor(Qt::darkCyan),
		QColor(Qt::darkMagenta),
		QColor(Qt::yellow),
		QColor("deepskyblue"),
		QColor("sandybrown"),
		QColor("violet"),
		QColor("silver"),
		QColor("royalblue"),
		QColor("palegreen"),
		QColor(Qt::white),
		QColor(Qt::black),
		QColor(Qt::transparent),
	};
	
	if (table) delete [] table;
	
	table = new QColor[colors];
	
	for (uint i = 0; i < colors; ++i)
		if (i < predefinedColors)
			table[i] = colortable[i];
		else
			table[i] = QColor::colorNames()[rand()%QColor::colorNames().size()];
}

//!Losuje kolor
BallColor BallColor::random()
{
	return rand()%colors;
}

//!Losuje kolor inny niz forbidden
BallColor BallColor::random(BallColor forbidden)
{
	BallColor res;
	while((res = random()) == forbidden);
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
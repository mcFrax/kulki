#include <cmath>

#include <QBrush>
#include <QPen>
#include <QPropertyAnimation>

#include "ballitem.hpp"
#include "square.hpp"

#include "debugtools.hpp"

const qreal BallItem::xmargin = 3;
const qreal BallItem::ymargin = 3;

BallItem::BallItem(const QColor& color)
	: QGraphicsEllipseItem()
{
	QGraphicsEllipseItem::setBrush(QBrush(color));
	setAcceptedMouseButtons(0);
}


int fallingDuration(qreal distance)
{
	#warning possible tuneable factor?
	return sqrt(distance)*180.0;
}

BallItem::BallItem(const QColor& color, Square* s, qreal yoffset)
	: QGraphicsEllipseItem(xmargin, yoffset+ymargin,
			Square::xSize-2*xmargin, Square::ySize-2*ymargin, s)
{
	QGraphicsEllipseItem::setBrush(QBrush(color));
	setAcceptedMouseButtons(0);
	
	if (yoffset != 0)
		animate(yoffset);
}

void BallItem::placeOnSquare(Square* s, qreal ypos)
{
	QGraphicsEllipseItem::setParentItem(s);
	//~ QGraphicsEllipseItem::setRect(xmargin, ypos+ymargin,
			//~ Square::xSize-2*xmargin, Square::ySize-2*ymargin);
	
	if (ypos != 0)
		animate(ypos);
}

void BallItem::animate(qreal yoffset)
{
	
	QPropertyAnimation* anim = new QPropertyAnimation(this, "rect");
	anim->setStartValue(rect());
	anim->setEndValue(QRectF(xmargin, ymargin,
			Square::xSize-2*xmargin, Square::ySize-2*ymargin));
	anim->setDuration(fallingDuration(fabs(yoffset)));
	anim->setEasingCurve(QEasingCurve::OutBounce);
	
	anim->start();
}

QBrush BallItem::brush() const
{
	return QGraphicsEllipseItem::brush();
}

void BallItem::setBrush(const QBrush& b)
{
	QGraphicsEllipseItem::setBrush(b);
}

#include "ballitem.hpp"
#include "square.hpp"
#include <QBrush>
#include <QPen>

const qreal BallItem::xmargin = 3;
const qreal BallItem::ymargin = 3;

BallItem::BallItem(const QColor& color)
	: QGraphicsEllipseItem()
{
	QGraphicsEllipseItem::setBrush(QBrush(color));
	setAcceptedMouseButtons(0);
}

#warning coordinates are not fully consistent yet

BallItem::BallItem(const QColor& color, Square* s, qreal xoffset,
		qreal yoffset)
	: QGraphicsEllipseItem(xoffset+xmargin, yoffset+ymargin,
			Square::xSize-2*xmargin, Square::ySize-2*ymargin, s)
{
	QGraphicsEllipseItem::setBrush(QBrush(color));
	setAcceptedMouseButtons(0);
}

void BallItem::placeOnSquare(Square* s, qreal xpos, qreal ypos)
{
	QGraphicsEllipseItem::setParentItem(s);
	QGraphicsEllipseItem::setRect(xpos+xmargin, ypos+ymargin,
			Square::xSize-2*xmargin, Square::ySize-2*ymargin);
}

void BallItem::setPosition(qreal x, qreal y)
{
	QGraphicsEllipseItem::setRect(x+xmargin, y+ymargin,
			Square::xSize-2*xmargin, Square::ySize-2*ymargin);
}

QBrush BallItem::brush() const
{
	return QGraphicsEllipseItem::brush();
}

void BallItem::setBrush(const QBrush& b)
{
	QGraphicsEllipseItem::setBrush(b);
}

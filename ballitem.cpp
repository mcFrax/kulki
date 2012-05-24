#include <cmath>

#include <QBrush>
#include <QPen>
#include <QPropertyAnimation>

#include "ballitem.hpp"
#include "square.hpp"
#include "board.hpp"

#include "debugtools.hpp"

const qreal BallItem::xmargin = 3;
const qreal BallItem::ymargin = 3;

//~ BallItem::BallItem(const QColor& color)
	//~ : QGraphicsEllipseItem()
//~ {
	//~ QGraphicsEllipseItem::setBrush(QBrush(color));
	//~ setAcceptedMouseButtons(0);
//~ }


int fallingDuration(qreal distance)
{
	#warning possible tuneable factor?
	return sqrt(distance)*180.0;
}

BallItem::BallItem(const QColor& color, Square* s, qreal yoffset, int animDelay)
	: QGraphicsEllipseItem(xmargin, yoffset+ymargin,
			Square::xSize-2*xmargin, Square::ySize-2*ymargin, s)
{
	QGraphicsEllipseItem::setBrush(QBrush(color));
	setAcceptedMouseButtons(0);
	
	if (yoffset != 0)
		animate(yoffset, animDelay);
}

void BallItem::placeOnSquare(Square* s, qreal ypos, int animDelay)
{
	QGraphicsEllipseItem::setParentItem(s);
	QGraphicsEllipseItem::setRect(xmargin, ypos+ymargin,
			Square::xSize-2*xmargin, Square::ySize-2*ymargin);
	
	if (ypos != 0)
		animate(ypos, animDelay);
}

void BallItem::animate(qreal yoffset, int animDelay)
{
	QPropertyAnimation* anim = new QPropertyAnimation(this, "rect");
	anim->setEndValue(QRectF(xmargin, ymargin,
			Square::xSize-2*xmargin, Square::ySize-2*ymargin));
	int duration = fallingDuration(fabs(yoffset)) + animDelay;
	anim->setKeyValueAt(animDelay/double(duration), rect());
	anim->setDuration(duration);
	anim->setEasingCurve(QEasingCurve::OutBounce);
	static_cast<Square*>(parentItem())->getBoard()->registerAnimation(anim);
	connect(anim, SIGNAL(finished()), static_cast<Square*>(parentItem())->
			getBoard(), SLOT(animationEnded()));
	
	anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void BallItem::explode()
{
	QPropertyAnimation* anim = new QPropertyAnimation(this, "rect");
	anim->setEndValue(QRectF(-10, -10,
			Square::xSize+20, Square::ySize+20));
	anim->setDuration(500);
	anim->setEasingCurve(QEasingCurve::InOutBack);
	connect(anim, SIGNAL(finished()), this, SLOT(deleteLater()));
	
	anim->start(QAbstractAnimation::DeleteWhenStopped);
}

QBrush BallItem::brush() const
{
	return QGraphicsEllipseItem::brush();
}

void BallItem::setBrush(const QBrush& b)
{
	QGraphicsEllipseItem::setBrush(b);
}

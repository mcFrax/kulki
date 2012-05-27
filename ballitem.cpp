#include <cmath>

#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QPropertyAnimation>
#include <QGraphicsPixmapItem>

#include "ballitem.hpp"
#include "square.hpp"
#include "board.hpp"

#include "debugtools.hpp"

const qreal BallItem::xmargin = 3;
const qreal BallItem::ymargin = 3;

int fallingDuration(qreal distance)
{
	return sqrt(distance/Square::ySize*64)*180.0;
}

QPixmap* BallItem::glossPixmap = 0;

BallItem::BallItem(const QColor& color, Square* s, qreal yoffset, int animDelay)
	: QGraphicsEllipseItem(xmargin, ymargin,
			Square::xSize-2*xmargin, Square::ySize-2*ymargin, s),
			specialPixmapItem(0)
{
	QGraphicsEllipseItem::setBrush(QBrush(color));
	setAcceptedMouseButtons(0);
	setTransformOriginPoint((Square::xSize-2*xmargin)/2,
			(Square::ySize-2*ymargin)/2);
	
	if (!glossPixmap)
		glossPixmap = new QPixmap(":Balls/Ball.png");
		
	glossPixmapItem = new QGraphicsPixmapItem(*glossPixmap, this);
	glossPixmapItem->setAcceptedMouseButtons(0);
	glossPixmapItem->setScale((Square::xSize-2*xmargin)/glossPixmap->width());
	glossPixmapItem->setZValue(1);
	glossPixmapItem->setPos(xmargin, ymargin);
	glossPixmapItem->setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
	glossPixmapItem->setTransformationMode(Qt::SmoothTransformation);
	
	if (yoffset != 0)
		animate(yoffset, animDelay);
}

void BallItem::placeOnSquare(Square* s, qreal ypos, int animDelay)
{
	QGraphicsEllipseItem::setParentItem(s);
	QGraphicsEllipseItem::setRect(xmargin, ymargin,
			Square::xSize-2*xmargin, Square::ySize-2*ymargin);
	
	if (ypos != 0)
		animate(ypos, animDelay);
}

void BallItem::placeOnSquare(Square* s, Square* from)
{
	QGraphicsEllipseItem::setParentItem(s);
	QGraphicsEllipseItem::setRect(xmargin, ymargin,
			Square::xSize-2*xmargin, Square::ySize-2*ymargin);
	
	animateArc(from->pos() - s->pos());
}

void BallItem::animate(qreal yoffset, int animDelay)
{
	QPropertyAnimation* anim = new QPropertyAnimation(this, "pos");
	anim->setStartValue(QPointF(0, yoffset));
	anim->setEndValue(QPointF(0, 0));
	int duration = fallingDuration(fabs(yoffset)) + animDelay;
	//~ anim->setKeyValueAt(animDelay/double(duration), rect());
	anim->setKeyValueAt(animDelay/double(duration), QPointF(0, yoffset));
	anim->setDuration(duration);
	anim->setEasingCurve(QEasingCurve::OutBounce);
	static_cast<Square*>(parentItem())->getBoard()->registerAnimation(anim);
	connect(anim, SIGNAL(finished()), static_cast<Square*>(parentItem())->
			getBoard(), SLOT(animationEnded()));
	
	anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void BallItem::animateArc(QPointF startPoint)
{
	QPropertyAnimation* anim = new QPropertyAnimation(this, "pos");
	anim->setStartValue(startPoint);
	anim->setEndValue(QPointF(0, 0));
	
	QLineF line(startPoint/2, startPoint);
	
	anim->setKeyValueAt(0.5, line.normalVector().p2());
	
	anim->setDuration(400);
	anim->setEasingCurve(QEasingCurve::InOutQuad);
	
	static_cast<Square*>(parentItem())->getBoard()->registerAnimation(anim);
	connect(anim, SIGNAL(finished()), static_cast<Square*>(parentItem())->
			getBoard(), SLOT(animationEnded()));
	
	anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void BallItem::explode()
{
	QPointF sp = scenePos();
	setParentItem(0);
	setPos(sp);
	setZValue(3);
	QPropertyAnimation* anim = new QPropertyAnimation(this, "scale");
	anim->setEndValue(2);
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

void BallItem::scalePixmapItem(const QPixmap& pm, qreal sizeFactor)
{
	if (pm.isNull()) return;
	qreal W = Square::xSize - 2*xmargin; //alternatywnie rect().width()
	qreal H = Square::ySize - 2*xmargin; //alternatywnie rect().height()
	qreal w = pm.width();
	qreal h = pm.height();
	qreal realScaleFactor = std::min((sizeFactor*W)/w, (sizeFactor*H)/h);
	specialPixmapItem->setScale(realScaleFactor);
	specialPixmapItem->setPos(xmargin+(W-w*realScaleFactor)/2, 
			ymargin+(H-h*realScaleFactor)/2);
}

void BallItem::setSpecialPixmap(const QPixmap& pm, qreal sizeFactor)
{
	if (specialPixmapItem) {
		specialPixmapItem->setPixmap(pm);
	} else {
		specialPixmapItem = new QGraphicsPixmapItem(pm, this);
		specialPixmapItem->setAcceptedMouseButtons(0);
		specialPixmapItem->setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
		specialPixmapItem->setTransformationMode(Qt::SmoothTransformation);
		specialPixmapItem->setZValue(2);
	}
	scalePixmapItem(pm, sizeFactor);
}

QPixmap BallItem::specialPixmap()
{
	if (specialPixmapItem)
		return specialPixmapItem->pixmap();
	else
		return QPixmap();
}
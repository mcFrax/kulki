#include <cmath>

#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QPropertyAnimation>
#include <QGraphicsPixmapItem>

#include "ballitem.hpp"
#include "square.hpp"
#include "board.hpp"

#include "settings.hpp"
#include "debugtools.hpp"

inline static int fallingDuration(qreal distance)
{
	return sqrt(distance/Square::size())*
		settings()->value("ball/fallingTimeFactor").toInt();
}

inline static qreal margin()
{
	return settings()->value("ball/margin").toDouble();
}

QPixmap* BallItem::glossPixmap = 0;

BallItem::BallItem(const QColor& color, Square* s, qreal yoffset, int animDelay)
	: QGraphicsEllipseItem(margin(), margin(),
			Square::size()-2*margin(), Square::size()-2*margin(), s->item()),
			specialPixmapItem(0)
{
	QGraphicsEllipseItem::setBrush(QBrush(color));
	setPen(Qt::NoPen);
	setAcceptedMouseButtons(0);
	setTransformOriginPoint((Square::size()-2*margin())/2,
			(Square::size()-2*margin())/2);
	
	if (!glossPixmap)
		glossPixmap = new QPixmap(":Balls/Ball.png");
		
	glossPixmapItem = new QGraphicsPixmapItem(*glossPixmap, this);
	glossPixmapItem->setAcceptedMouseButtons(0);
	glossPixmapItem->setScale((Square::size()-2*margin())/glossPixmap->width());
	glossPixmapItem->setZValue(1);
	glossPixmapItem->setPos(margin(), margin());
	glossPixmapItem->setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
	glossPixmapItem->setTransformationMode(Qt::SmoothTransformation);
	
	if (yoffset != 0)
		animateFalling(yoffset, animDelay);
	else
		animateAppear(animDelay);
}

void BallItem::placeOnSquare(Square* s, qreal ypos, int animDelay)
{
	QGraphicsEllipseItem::setParentItem(s->item());
	QGraphicsEllipseItem::setRect(margin(), margin(),
			Square::size()-2*margin(), Square::size()-2*margin());
	
	if (ypos != 0)
		animateFalling(ypos, animDelay);
}

void BallItem::placeOnSquare(Square* s, Square* from)
{
	QGraphicsEllipseItem::setParentItem(s->item());
	QGraphicsEllipseItem::setRect(margin(), margin(),
			Square::size()-2*margin(), Square::size()-2*margin());
	
	animateArc(from->item()->pos() - s->item()->pos());
}

void BallItem::animateAppear(int animDelay)
{
	const int appearTime = settings()->value("ball/appearTime").toInt();
	QPropertyAnimation* anim = new QPropertyAnimation(this, "opacity");
	anim->setStartValue(0);
	anim->setEndValue(1);
	int duration = appearTime+animDelay;
	anim->setKeyValueAt(animDelay/double(duration), 0);
	anim->setDuration(duration);
	anim->setEasingCurve(QEasingCurve::InQuad);
	
	static_cast<Board*>(parentItem()->parentItem())->
			registerAnimation(anim);
	
	anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void BallItem::animateFalling(qreal yoffset, int animDelay)
{
	QPropertyAnimation* anim = new QPropertyAnimation(this, "pos");
	anim->setStartValue(QPointF(0, yoffset));
	anim->setEndValue(QPointF(0, 0));
	int duration = fallingDuration(fabs(yoffset)) + animDelay;
	anim->setKeyValueAt(animDelay/double(duration), QPointF(0, yoffset));
	anim->setDuration(duration);
	//~ anim->setEasingCurve(QEasingCurve::OutBounce);
	anim->setEasingCurve(QEasingCurve::InQuad);
	
	static_cast<Board*>(parentItem()->parentItem())->
			registerAnimation(anim);
	
	anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void BallItem::animateArc(QPointF startPoint)
{
	QPropertyAnimation* anim = new QPropertyAnimation(this, "pos");
	anim->setStartValue(startPoint);
	anim->setEndValue(QPointF(0, 0));
	
	QLineF line(startPoint/2, startPoint);
	
	anim->setKeyValueAt(0.5, line.normalVector().p2());
	
	anim->setDuration(settings()->value("ball/arcTime").toInt());
	anim->setEasingCurve(QEasingCurve::InOutQuad);
	
	static_cast<Board*>(parentItem()->parentItem())->
			registerAnimation(anim);
	
	anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void BallItem::explode()
{
	QPointF sp = pos()+parentItem()->pos(); //wzgledem planszy
	setParentItem(parentItem()->parentItem());
	setPos(sp);
	setZValue(3);
	QPropertyAnimation* anim = new QPropertyAnimation(this, "scale");
	anim->setEndValue(settings()->value("ball/explosionFactor").toDouble());
	anim->setDuration(settings()->value("ball/explosionTime").toInt());
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
	qreal W = Square::size() - 2*margin(); //alternatywnie rect().width()
	qreal H = Square::size() - 2*margin(); //alternatywnie rect().height()
	qreal w = pm.width();
	qreal h = pm.height();
	qreal realScaleFactor = std::min((sizeFactor*W)/w, (sizeFactor*H)/h);
	specialPixmapItem->setScale(realScaleFactor);
	specialPixmapItem->setPos(margin()+(W-w*realScaleFactor)/2, 
			margin()+(H-h*realScaleFactor)/2);
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
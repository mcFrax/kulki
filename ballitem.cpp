#include <cmath>

#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QPropertyAnimation>
#include <QGraphicsPixmapItem>

#include "ballitem.hpp"
#include "square.hpp"
#include "board.hpp"
#include "gloss.hpp"

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

QImage BallItem::glossImage;
QImage BallItem::maskImage;

BallItem::BallItem(const QColor& color, Square* s, qreal yoffset, int animDelay)
	: QGraphicsPixmapItem(getPixmap(color), s->item()), specialPixmapItem(0)
{
	//~ setOffset(margin(), margin());
	setAcceptedMouseButtons(0);
	setScale((Square::size()-2*margin())/pixmap().width());
	setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
	setTransformationMode(Qt::SmoothTransformation);
		
	//~ setTransformOriginPoint(boundingRect().center() + offset());
	
	if (yoffset != 0)
		animateFalling(yoffset, animDelay);
	else
		animateAppear(animDelay);
}

inline void BallItem::commonGetPixmap()
{
	if (glossImage.isNull())
		glossImage.load(":Balls/BallGloss.png");
	if (maskImage.isNull())
		maskImage.load(":Balls/BallMask.png");
}

QPixmap BallItem::getPixmap(QColor color)
{
	commonGetPixmap();
	return QPixmap::fromImage(glossyColorShape(color, maskImage, glossImage));
}

QPixmap BallItem::getPixmap(QBrush brush)
{
	commonGetPixmap();
	return QPixmap::fromImage(glossyColorShape(brush, maskImage, glossImage));
}

void BallItem::placeOnSquare(Square* s, qreal ypos, int animDelay)
{
	setParentItem(s->item());
	
	if (ypos != 0)
		animateFalling(ypos, animDelay);
}

void BallItem::placeOnSquare(Square* s, Square* from)
{
	setParentItem(s->item());
	
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
	anim->setEndValue(settings()->value("ball/explosionFactor").toDouble()*scale());
	anim->setDuration(settings()->value("ball/explosionTime").toInt());
	anim->setEasingCurve(QEasingCurve::InOutBack);
	connect(anim, SIGNAL(finished()), this, SLOT(deleteLater()));
	
	anim->start(QAbstractAnimation::DeleteWhenStopped);
}

//~ QBrush BallItem::brush() const
//~ {
	//~ return QGraphicsEllipseItem::brush();
//~ }

void BallItem::setBrush(const QBrush& b)
{
	setPixmap(getPixmap(b));
}

void BallItem::scalePixmapItem(const QPixmap& pm, qreal sizeFactor)
{
	if (pm.isNull()) return;
	qreal W = pixmap().width();//Square::size() - 2*margin(); //alternatywnie rect().width()
	qreal H = pixmap().height();//Square::size() - 2*margin(); //alternatywnie rect().height()
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
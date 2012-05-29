#include <cmath>

#include <QBrush>
#include <QGraphicsPixmapItem>
#include <QHash>
#include <QPainter>
#include <QPen>
#include <QPixmap>
#include <QPropertyAnimation>

#include "ballitem.hpp"
#include "board.hpp"
#include "gloss.hpp"
#include "square.hpp"

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
	: QGraphicsItem(s->item()), 
		rect(margin(), margin(), Square::size()-margin(),
				Square::size()-margin()),
		pixmapVal(getPixmap(color))
{
	setAcceptedMouseButtons(0);
		
	setTransformOriginPoint(Square::size()/2 - margin(), 
			Square::size()/2 - margin());
	
	if (yoffset != 0)
		animateFalling(yoffset, animDelay);
	else
		animateAppear(animDelay);
}

QRectF BallItem::boundingRect() const
{
	return rect;
}

void BallItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
	#warning to powinno byc gdzies indziej
	painter->setRenderHint(QPainter::SmoothPixmapTransform);
	
	painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
	painter->drawPixmap(rect, pixmapVal, pixmapVal.rect());
	painter->drawPixmap(specialPixmapRect, specialPixmapVal, specialPixmapVal.rect());
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
	static QHash<QRgb, QPixmap> map;
	QHash<QRgb, QPixmap>::iterator it = map.find(color.rgba());
	if (it == map.end()){
		commonGetPixmap();
		it = map.insert(
				color.rgba(),
				QPixmap::fromImage(
						glossyColorShape(color, maskImage, glossImage))
				);
	}
	return it.value();
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

//~ QBrush BallItem::brush()
//~ {
	//~ return QGraphicsEllipseItem::brush();
//~ }

void BallItem::setBrush(const QBrush& b)
{
	pixmapVal = getPixmap(b);
	update();
}

void BallItem::setSpecialPixmap(const QPixmap& pm, qreal sizeFactor)
{
	specialPixmapVal = pm;
	qreal margin = (rect.width() - rect.width()*sizeFactor) / 2;
	specialPixmapRect = rect.adjusted(margin, margin, -margin, -margin);
}

QPixmap BallItem::specialPixmap()
{
	return specialPixmapVal;
}
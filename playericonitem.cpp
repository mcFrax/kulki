#include "playericonitem.hpp"

#include <QBrush>
#include <QColor>
#include <QPropertyAnimation>

//~ #include "debugtools.hpp"

PlayerIconItem::PlayerIconItem(QColor color, QGraphicsItem* parentItem)
	: QGraphicsEllipseItem(0,0,32,32,parentItem), color(color), colornessVal(0)
{
	setTransformOriginPoint(16,16);
	
	throbberAnim = new QPropertyAnimation(this, "scale");
	throbberAnim->setDuration(750);
	throbberAnim->setKeyValueAt(0.5, 0.9);
	throbberAnim->setEndValue(1);
	throbberAnim->setLoopCount(-1);
	
	setColorness(0.3);
}

PlayerIconItem::~PlayerIconItem()
{
	delete throbberAnim;
}

qreal PlayerIconItem::colorness()
{
	return colornessVal;
}

static inline QColor avgColor(const QColor& c1, const QColor& c2, 
		qreal w1, qreal w2)
{
	return QColor::fromRgbF(
			c1.redF()  * w1  +  c2.redF()  * w2,
			c1.greenF()* w1  +  c2.greenF()* w2,
			c1.blueF() * w1  +  c2.blueF() * w2 );
}

void PlayerIconItem::setColorness(qreal v)
{
	colornessVal = v;
	setBrush(avgColor(color, Qt::lightGray, v, 1-v));
}

void PlayerIconItem::blink()
{
	throbberAnim->stop();
	setScale(1);
	
	QPropertyAnimation* anim = new QPropertyAnimation(this, "scale");
	anim->setDuration(400);
	anim->setKeyValueAt(0.5, 1.4);
	anim->setEndValue(1);
	
	anim->setEasingCurve(QEasingCurve::InOutQuad);
	
	anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void PlayerIconItem::activate()
{
	throbberAnim->stop();
	
	QPropertyAnimation* anim = new QPropertyAnimation(this, "colorness");
	anim->setDuration(400);
	anim->setEndValue(1);
	
	anim->setEasingCurve(QEasingCurve::InQuad);
	
	anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void PlayerIconItem::deactivate()
{
	throbberAnim->stop();
	setScale(1);
	
	QPropertyAnimation* anim = new QPropertyAnimation(this, "colorness");
	anim->setDuration(400);
	anim->setEndValue(0.3);
	
	anim->setEasingCurve(QEasingCurve::InQuad);
	
	anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void PlayerIconItem::throbber()
{
	throbberAnim->start();
}
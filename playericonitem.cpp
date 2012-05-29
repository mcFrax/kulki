#include "playericonitem.hpp"
#include "gloss.hpp"

#include <QBrush>
#include <QColor>
#include <QImage>
#include <QPainter>
#include <QPropertyAnimation>

//~ #include "debugtools.hpp"

QImage PlayerIconItem::glossImage;
QImage PlayerIconItem::maskImage;

PlayerIconItem::PlayerIconItem(QColor color, QGraphicsItem* parentItem)
	: QGraphicsItem(parentItem), pixmap(getPixmap(color)), color(color)
{
	setTransformOriginPoint(16, 16);
	
	throbberAnim = new QPropertyAnimation(this, "scale");
	throbberAnim->setDuration(2000);
	throbberAnim->setKeyValueAt(0.5, 0.9);
	throbberAnim->setEndValue(1);
	throbberAnim->setLoopCount(-1);
	
	//~ throbberAnim->setEasingCurve(QEasingCurve::InOutQuad);
	
	setOpacity(0.3);
}

PlayerIconItem::~PlayerIconItem()
{
	delete throbberAnim;
}

QRectF PlayerIconItem::boundingRect() const
{
	return QRectF(0,0,32,32);
}

QPixmap PlayerIconItem::getPixmap(QColor color)
{
	if (glossImage.isNull())
		glossImage.load(":SquareGloss.png");
	if (maskImage.isNull())
		maskImage.load(":SquareMask.png");
	return QPixmap::fromImage(glossyColorShape(color, maskImage, glossImage));
}

void PlayerIconItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
	painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
	painter->drawPixmap(QRect(0,0,32,32), pixmap);
}

void PlayerIconItem::blink()
{
	blink(0);
}

void PlayerIconItem::blink(bool throbber)
{
	throbberAnim->stop();
	setScale(1);
	
	QPropertyAnimation* anim = new QPropertyAnimation(this, "scale");
	anim->setDuration(800);
	anim->setKeyValueAt(0.5, 1.4);
	anim->setEndValue(1);
	
	anim->setEasingCurve(QEasingCurve::InQuad);
	
	if (throbber)
		connect(anim, SIGNAL(finished()), throbberAnim, SLOT(start()));
	
	anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void PlayerIconItem::activate()
{
	throbberAnim->stop();
	
	QPropertyAnimation* anim = new QPropertyAnimation(this, "opacity");
	anim->setDuration(400);
	anim->setEndValue(1);
	
	anim->setEasingCurve(QEasingCurve::InQuad);
	
	anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void PlayerIconItem::deactivate()
{
	throbberAnim->stop();
	setScale(1);
	
	QPropertyAnimation* anim = new QPropertyAnimation(this, "opacity");
	anim->setDuration(400);
	anim->setEndValue(0.3);
	
	anim->setEasingCurve(QEasingCurve::InQuad);
	
	anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void PlayerIconItem::throbber()
{
	throbberAnim->start();
}

void PlayerIconItem::noThrobber()
{
	throbberAnim->stop();
}
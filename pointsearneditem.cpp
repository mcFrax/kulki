#include "pointsearneditem.hpp"
#include "board.hpp"

#include <QFont>
#include <QBrush>
#include <QString>
#include <QPropertyAnimation>

#include "debugtools.hpp"

PointsEarnedItem::PointsEarnedItem(Board* b, qreal x, qreal y, int points)
	: QGraphicsSimpleTextItem(QString::number(points), 0, b),
		posVal(0), sizeVal(0)
{
	//setFont(###);
	QRectF r = boundingRect();
	this->x = x;
	this->y = y-r.height()/2;
	QGraphicsSimpleTextItem::setPos(x-r.width()/2, this->y);
	
	setBrush(QBrush(QRadialGradient()));
	setPen(QColor(Qt::black));
	
	setAcceptedMouseButtons(0);
	setZValue(5);
	
	QPropertyAnimation* anim = new QPropertyAnimation(this, "pos");
	anim->setDuration(2500);
	anim->setStartValue(0);
	anim->setEndValue(1);
	connect(anim, SIGNAL(finished()), this, SLOT(deleteLater()));
	anim->start(QAbstractAnimation::DeleteWhenStopped);
	
	
	qreal startVal = font().pointSizeF();
	QPropertyAnimation* anim2 = new QPropertyAnimation(this, "size");
	anim2->setDuration(2500);
	anim2->setStartValue(startVal);
	anim2->setKeyValueAt(0.05, startVal*6);
	anim2->setEndValue(startVal);
	anim2->start(QAbstractAnimation::DeleteWhenStopped);
}

qreal PointsEarnedItem::pos()
{
	return posVal;
}

void PointsEarnedItem::setPos(qreal val)
{
	posVal = val;
	
	QGraphicsSimpleTextItem::setPos(
			x-boundingRect().width()/2, y-posVal*80);
}

qreal PointsEarnedItem::size()
{
	return sizeVal;
}

void PointsEarnedItem::setSize(qreal val)
{
	sizeVal = val;
	qreal h = boundingRect().height();
	QFont f = font();
	f.setPointSizeF(val);
	setFont(f);
	y -= boundingRect().height() - h;
	
	QGraphicsSimpleTextItem::setPos(
			x-boundingRect().width()/2, y-posVal*80);
}

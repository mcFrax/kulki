#include "highlightitem.hpp"
#include "board.hpp"
#include "square.hpp"

#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QImage>

#include "settings.hpp"
#include "debugtools.hpp"

static inline qreal WFactor()
{
	return settings()->value("highlight/widthFactor").toDouble();
}

QPixmap* HighlightItem::pixmap[2] = {0};

HighlightItem::HighlightItem(qreal x, qreal y, Board* board, Square* s1, Square* s2,
		Direction dir)
	: QGraphicsPixmapItem(getPixmap(dir), board), board(board), s1(s1), s2(s2),
		direction(dir), visibilityVal(0), anim(0), active(0)
{
	qreal scaleF;
	if (dir == horizontal)
		scaleF = WFactor()*Square::size()/getPixmap(dir).height();
	else
		scaleF = WFactor()*Square::size()/getPixmap(dir).width();
	setScale(scaleF);
	setZValue(1);
	setPos(x-scaleF*getPixmap(dir).width()/2, 
			y-scaleF*getPixmap(dir).height()/2);
	//nie jestem pewien, czy ten efekt jest potrzebny, ale uzycie zwyklego
	//opacity jakos nawalalo - przezroczysty element nie dostawal mouseEventow
	setGraphicsEffect(new QGraphicsOpacityEffect());
	setVisibility(0);
	setVisible(0);
	setAcceptHoverEvents(1);
};

QPixmap HighlightItem::getPixmap(Direction dir)
{
	if (!pixmap[dir]){
		if (dir == vertical){
			QImage img(":Highlight.png");
			pixmap[dir] = new QPixmap(QPixmap::fromImage(
					img.transformed(QTransform().rotate(90))));
		} else {
			pixmap[dir] = new QPixmap(":Highlight.png");
		}
	}
	return *pixmap[dir];
}

void HighlightItem::animFinished()
{
	anim = 0;
}

void HighlightItem::setVisibility(qreal v)
{
	visibilityVal = v;
	
	static_cast<QGraphicsOpacityEffect*>(graphicsEffect())->setOpacity(
			settings()->value("highlight/opacityFactor").toDouble()*v);
}

qreal HighlightItem::visibility()
{
	return visibilityVal;
}

void HighlightItem::hoverMoveEvent(QGraphicsSceneHoverEvent * event)
{
	if (board->isLegal(s1, s2))
		activate();
}

void HighlightItem::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
	deactivate();
}

void HighlightItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	if (anim) {
		delete anim;
		anim = 0;
	}
	active = 0;
	setVisibility(0);
	board->move(s1, s2);
	//~ deactivate();
}

void HighlightItem::activate()
{
	if (active) return;
	active = 1;
	if (anim) {
		delete anim;
		anim = 0;
	}
	setVisibility(1);
}

void HighlightItem::deactivate()
{
	if (!active) return;
	active = 0;
	anim = new QPropertyAnimation(this, "visibility");
	anim->setEndValue(0);
	anim->setDuration(350);
	//~ anim->setEasingCurve(QEasingCurve::InBack);
	connect(anim, SIGNAL(finished()), this, SLOT(animFinished()));
	anim->start(QAbstractAnimation::DeleteWhenStopped);
}
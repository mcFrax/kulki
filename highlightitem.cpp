#include "highlightitem.hpp"
#include "board.hpp"

#include <QPropertyAnimation>

static const qreal itL = 10;
static const qreal itW = 8;

HighlightItem::HighlightItem(qreal x, qreal y, Board* board, Square* s1, Square* s2,
		Direction dir)
	: QGraphicsEllipseItem(0, 0, dir?itL:itW, dir?itL:itH, board),
		board(board), s1(s1), s2(s2), direction(dir), visibilityVal(1), anim(0)
{
	setPos(x-width()/2, y-height()/2);
	setVisibility(0);
	setPen(Qt::NoPen);
};

void HighlightItem::setVisibility(qreal v)
{
	visibilityVal = v;
	setBrush(QBrush(QColor(0,0,0,127*v)));
}

qreal HighlightItem::visibility()
{
	return visibilityVal;
}

void HighlightItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	board->move(s1, s2);
	deactivate();
}

void HighlightItem::activate()
{
	if (anim) {
		delete anim;
		anim = 0;
	}
	setVisibility(1);
}

void HighlightItem::deactivate()
{
	anim = new QPropertyAnimation(this, "visibility");
	anim->setEndValue(0);
	anim->setDuration(800);
	anim->setEasingCurve(QEasingCurve::InBack);
	//~ connect(anim, SIGNAL(finished()), this, SLOT(deleteLater()));
	anim->start(QAbstractAnimation::DeleteWhenStopped);
}
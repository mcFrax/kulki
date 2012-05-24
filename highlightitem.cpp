#include "highlightitem.hpp"
#include "board.hpp"

#include <QPropertyAnimation>

#include "debugtools.hpp"

static const qreal itL = 20;
static const qreal itW = 16;

HighlightItem::HighlightItem(qreal x, qreal y, Board* board, Square* s1, Square* s2,
		Direction dir)
	: QGraphicsEllipseItem(0, 0, dir?itL:itW, dir?itW:itL, 0, board),
		board(board), s1(s1), s2(s2), direction(dir), visibilityVal(1),
		anim(0), active(0)
{
	setPos(x-(dir?itL:itW)/2, y-(dir?itW:itL)/2);
	setVisibility(0);
	setPen(Qt::NoPen);
	setAcceptHoverEvents(1);
	setVisible(0);
};

void HighlightItem::animFinished()
{
	anim = 0;
}

void HighlightItem::setVisibility(qreal v)
{
	visibilityVal = v;
	setBrush(QBrush(QColor(0,0,0,127*v)));
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
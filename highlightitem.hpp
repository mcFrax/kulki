#pragma once
#include <QGraphicsEllipseItem>

class Board;
class Square;
class QPropertyAnimation;

class HighlightItem : public QObject, public QGraphicsEllipseItem
{
	Q_OBJECT
	Q_PROPERTY(qreal visibility READ visibility WRITE setVisibility)
	public:
		enum Direction
		{
			vertical = 0,
			horizontal = 1
		};
	private:
		Board* board;
		Square* s1;
		Square* s2;
		qreal visibilityVal;
		qreal visibility();
		void setVisibility(qreal);
		Direction direction;
		QPropertyAnimation* anim;
		bool active;
	private slots:
		void animFinished();
	protected:
		void mousePressEvent(QGraphicsSceneMouseEvent * event);
		void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);
		void hoverMoveEvent(QGraphicsSceneHoverEvent * event);
	public:
		HighlightItem(qreal x, qreal y, Board*, Square* s1, Square* s2,
				Direction);
	public slots:
		void activate();
		void deactivate();
};
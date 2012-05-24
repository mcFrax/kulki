#pragma once
#inlude <QGraphicsEllipseItem>

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
			horizontal = 0,
			vertical = 1
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
	protected:
		void mousePressEvent(QGraphicsSceneMouseEvent * event);
	public:
		HighlightItem(qreal x, qreal y, Board*, Square* s1, Square* s2,
				Direction);
	public slots:
		void activate();
		void deactivate();
};
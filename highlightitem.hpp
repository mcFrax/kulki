#pragma once
#include <QGraphicsPixmapItem>

class Board;
class Square;
class QPixmap;
class QPropertyAnimation;

class HighlightItem : public QObject, public QGraphicsPixmapItem
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
		Direction direction;
		qreal visibilityVal;
		qreal visibility();
		void setVisibility(qreal);
		QPropertyAnimation* anim;
		bool active;
		static QPixmap* pixmap[2];
	private:
		static QPixmap getPixmap(Direction);
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
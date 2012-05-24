#pragma once
#inlude <QGraphicsEllipseItem>

class Board;
class Square;

class HighlightItem : public QObject, public QGraphicsEllipseItem
{
	Q_OBJECT
	Q_PROPERTY(qreal visibility READ visibility WRITE setVisibility)
	public:
		enum Direction
		{
			horizontal,
			vertical
		};
	private:
		Board* board;
		Square* s1;
		Square* s2;
		qreal visibilityVal;
		qreal visibility();
		void setVisibility(qreal);
	protected:
		
	public:
		HighlightItem(qreal x, qreal y, Board*, Square* s1, Square* s2,
				Direction, qreal visibility = 0);
};
#pragma once
#include <QGraphicsRectItem>

class Board;
class Ball;
class BallColor;

class Square : public QObject, public QGraphicsRectItem
{
	Q_OBJECT
	friend class Ball;
	public:
		enum Side {
			left = 0,
			top = 1,
			right = 2,
			bottom = 3,
			none = 4
		};
	public:
		const uint xPos;
		const uint yPos;
	protected:
		Ball* ball;
		Board* board;
		Square* neighbours[4];
		QGraphicsItem* highlight;
		Square::Side highlightedSide;
	protected:
		void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);
		void hoverMoveEvent(QGraphicsSceneHoverEvent * event);
		void setHighlight(bool, Square::Side side = left);
		Square::Side side(QPointF);
	public:
		Square(uint xPos, uint yPos, Board*); //pos is in squares
		void setNeighbours(Square* l, Square* t, Square* r, Square* b);
		Square* getNeighbour(Square::Side);
		Square*const* getNeighbours() const;
		Ball* getBall();
		Board* getBoard();
		BallColor ballColor();
		
		static const qreal xSize;
		static const qreal ySize;
	public slots:
		
};
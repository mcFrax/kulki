#pragma once
#include <QGraphicsRectItem>

class Board;
class Ball;
class BallColor;
class HighlightItem;
class QBrush;

class Square : public QGraphicsRectItem
{
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
	public:
		Square(uint xPos, uint yPos, Board*); //pos is in squares
		void setNeighbours(Square* l, Square* t, Square* r, Square* b);
		Square* getNeighbour(Square::Side);
		Square*const* getNeighbours() const;
		Ball* getBall();
		Board* getBoard();
		BallColor ballColor();
		#warning nazwy sie gryza, ale to chyba powinno byc tutaj:
		void takeBall(int animDelay = 0);
		
		static const qreal xSize;
		static const qreal ySize;
};
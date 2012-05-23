#pragma once
#include <QGraphicsEllipseItem>
#include <QDateTime>

#include "ballcolor.hpp"
#include "board.hpp"

class Square;
class BallItem;

class Ball
{
	public: 
		enum State {
			normal,
			falling,
			none,
		};
	private: 
		//~ qreal positionOnSqare;
		//~ int fallingStart; //in game time
		//~ static const qreal fallingTime;
		BallColor color;
		Square* square;
		State state;
	protected:
		BallItem* ballItem;
		void setColor(const BallColor&);
	protected: 
		//~ Ball(const BallColor&);
		Ball(const BallColor&, Square*, int falling = 0);
	public: 
		~Ball();
		virtual void placeOnSquare(Square*, int falling = 0);
		virtual uint getPointValue() = 0;
		virtual int applyPointModificator(const int& points) const;
		QRectF getRect();
		BallColor getColor() const;
		//~ BallItem* getBallItem();
		virtual void update(uint plyNumber);
	public:
		//~ static Ball* getNew(const Board::GameSetup&);
		static Ball* getNew(const Board::GameSetup&, Square* s,
				int falling = 0);
};

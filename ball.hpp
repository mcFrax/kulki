#pragma once
#include <QGraphicsEllipseItem>
#include <QList>

#include "ballcolor.hpp"
#include "board.hpp"

class Square;
class BallItem;

class Ball
{
	private: 
		BallColor color;
	protected:
		Square* squareVal;
		BallItem* ballItem;
		void setColor(const BallColor&);
	protected:
		Ball(const BallColor&, Square*, int falling = 0, int animDelay = 0);
	public: 
		~Ball();
		virtual void placeOnSquare(Square*, int falling, int animDelay = 0);
		virtual void placeOnSquare(Square*, Square* from);
		virtual void placeOnSquare(Square*);
		virtual uint getPointValue() = 0;
		virtual int applyPointModificator(const int& points) const;
		BallColor getColor() const;
		virtual void newTurnUpdate();
		virtual void newCheckUpdate();
		void explode(bool immediately = false);
		void detach();
		void appear(int animDelay = 0);
		Square* square();
	public:
		static Ball* getNew(const Board::GameSetup&, Square* s,
				int falling = 0, int animDelay = 0);
		static qreal levelFactor(const QList<uint> ballTypeSettings);
		static const int specialBallTypes; //liczba typow kulek
		static const QString specialBallTypeNames[]; //liczba typow kulek
};

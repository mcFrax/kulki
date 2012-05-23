#pragma once

#include "board.hpp"

class BallColor;

//Tu sa upchniete te metody pomocnicze, ktore niepotrzebnie zasmiecaly
//Board, a wyrzucenie ich poza klase byloby problemem.
class BoardImplementation : public Board
{
	Q_OBJECT
	protected:
		Square** squares;
		Square* square(uint x, uint y);
	protected:
		void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);
		void mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent);
		void check();
		void computeLegalMoves(const int, const int);
		bool computeLegalMoves();
		uint countSame(uint sX, uint sY, bool xM, bool yM, BallColor bc);
		void refill();
	private:
		bool move(Square*, Square*);
	public:
		BoardImplementation(const GameSetup&, QObject * parent = 0);
		~BoardImplementation();
	public slots:
		void setCurrentPlayer(Player*);
};
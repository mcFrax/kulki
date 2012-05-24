#pragma once
#include <vector>

#include "board.hpp"
#include "array2.hpp"

class Ball;
class BallColor;
class HighlightItem;

//Tu sa upchniete te metody pomocnicze, ktore niepotrzebnie zasmiecaly
//Board, a wyrzucenie ich poza klase byloby problemem.
class BoardImplementation : public Board
{
	Q_OBJECT
	private:
		class Row : public std::vector<Ball*>
		{
			public:
				int points() const;
				bool matches(Ball*) const;
		};
		typedef std::vector<Row> Rows;
	private:
		Array2<Square*> squares;
		Array2<std::pair<HighlightItem*, HighlightItem*> > highlights;
		int total;
	private:
		void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);
		void mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent);
		void check();
		void computeLegalMoves(const int, const int);
		bool computeLegalMoves();
		Rows findRows();
		void refill();
		void newPly();
	private:
		bool move(Square*, Square*);
	public:
		BoardImplementation(const GameSetup&, QObject * parent = 0);
		~BoardImplementation();
	public slots:
		void setCurrentPlayer(Player*);
};
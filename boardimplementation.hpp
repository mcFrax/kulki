#pragma once
#include <vector>

#include "board.hpp"

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
			private:
				int sum;
				bool sumUpToDate;
			public:
				Row();
				int points() const;
				bool matches(Ball*) const;
				void emitPointsEarnedItem(Board*) const;
		};
		typedef std::vector<Row> Rows;
	private:
		Array2<std::pair<HighlightItem*, HighlightItem*> > highlights;
		int total;
		int turnNumber;
		bool swappingBalls;
	private:
		void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);
		void mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent);
		void check();
		void computeLegalMoves(const int, const int, bool);
		bool computeLegalMoves(bool);
		Rows findRows();
		void refill();
		void newTurn();
		void ballsNewCheckUpdate();
	private:
		bool move(Square*, Square*);
	public:
		BoardImplementation(const GameSetup&, QObject * parent = 0);
		~BoardImplementation();
	public slots:
		void setCurrentPlayer(Player*);
};
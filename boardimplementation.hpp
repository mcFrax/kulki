#pragma once
#include <vector>

#include "board.hpp"

class Ball;
class BallColor;
class HighlightItem;

//Tu sa upchniete wewnetrzne metody, zeby: a) nie zasmiecac interfejsu, b) podzielic kod
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
	private:
		void check();
		void computeLegalMoves(const int, const int, bool);
		bool computeLegalMoves(bool);
		Rows findRows();
		void gravity();
		void refill(int animDelay = 0);
		void newTurn();
		void ballsNewCheckUpdate();
	private:
		bool move(Square*, Square*);
	public:
		BoardImplementation(const GameSetup&, QGraphicsItem * parent = 0);
		~BoardImplementation();
	public slots:
		void setCurrentPlayer(Player*);
};
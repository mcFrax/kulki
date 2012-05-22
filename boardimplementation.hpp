#pragma once
#include <set>
#include "board.hpp"

class BoardImplementation : public Board
{
	Q_OBJECT
	protected:
		Square** squares;
		Square* square(uint x, uint y);
		std::set<std::pair<Square*, Square*> > legalMoves;
	protected:
		void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);
		void check();
		void computeLegalMoves(int, int);
		void countSame();
		void computeLegalMoves();
	private slots:
		void squarePressed(Square*, Square*);
	public:
		BoardImplementation(const GameSetup&, QObject * parent = 0);
		~BoardImplementation();
	public slots:
		void setCurrentPlayer(Player*);
};
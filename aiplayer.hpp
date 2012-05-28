#pragma once

#include <queue>

#include "player.hpp"

class QColor;

class AIPlayer : public Player
{
	Q_OBJECT
	private:
		std::queue<Board*> boards;
		void makeMove(Board*);
	private slots:
		void makeRealMove();
	public:
		AIPlayer(QColor color);
		bool isHuman() const;
};
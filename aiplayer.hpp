#pragma once

#include <queue>

#include "player.hpp"

class AIPlayer : public Player
{
	Q_OBJECT
	private:
		std::queue<Board*> boards;
		void makeMove(Board*);
	private slots:
		void makeRealMove();
	public:
		AIPlayer();
		bool isHuman() const;
};
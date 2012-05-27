#pragma once

#include "player.hpp"

class HumanPlayer : public Player
{
	private:
		void makeMove(Board*);
	public:
		HumanPlayer(QString name);
		bool isHuman() const;
};
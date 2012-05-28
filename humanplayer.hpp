#pragma once

#include "player.hpp"

class QColor;

class HumanPlayer : public Player
{
	private:
		void makeMove(Board*);
	public:
		HumanPlayer(QString name, QColor color);
		bool isHuman() const;
};
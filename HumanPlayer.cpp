#include "HumanPlayer.hpp"

HumanPlayer::HumanPlayer(QString name, QColor color, QObject* parent)
	: Player(name, color, 0, parent)
{
}

void HumanPlayer::makeMove(Board*)
{
}

bool HumanPlayer::isHuman() const
{
	return 1;
}
#include "humanplayer.hpp"

HumanPlayer::HumanPlayer(QString name, QColor color)
	: Player(name, color)
{
}

void HumanPlayer::makeMove(Board*)
{
}

bool HumanPlayer::isHuman() const
{
	return 1;
}
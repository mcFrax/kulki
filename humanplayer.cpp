#include "humanplayer.hpp"

HumanPlayer::HumanPlayer(QString name)
	: Player(name)
{
}

void HumanPlayer::makeMove(Board*)
{
}

bool HumanPlayer::isHuman() const
{
	return 1;
}
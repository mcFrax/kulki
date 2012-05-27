#include "player.hpp"

Player::Player(QString name)
	: nameVal(name), pointsVal(0)
{
}

int Player::points()
{
	return pointsVal;
}

QString Player::name()
{
	return nameVal;
}
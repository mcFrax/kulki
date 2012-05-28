#include "player.hpp"

Player::Player(QString name, QColor color)
	: nameVal(name), pointsVal(0), colorVal(color)
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

QColor Player::color()
{
	return colorVal;
}

Player::PlayerInfo::PlayerInfo ()
{
}

static Player::PlayerInfo::Type typeOf(QString s)
{
	if (s == "Human")
		return Player::PlayerInfo::human;
	if (s == "Cpu")
		return Player::PlayerInfo::cpu;
	return Player::PlayerInfo::unknown;
}

static QString typeString(Player::PlayerInfo::Type t)
{
	switch (t) {
		case Player::PlayerInfo::human: return "Human";
		case Player::PlayerInfo::cpu: return "Cpu";
		case Player::PlayerInfo::unknown: return "Unknown";
	}
}

Player::PlayerInfo::PlayerInfo (QString name, QString type, QColor color)
	: name(name), type(typeOf(type)), color(color)
{
}

Player::PlayerInfo::PlayerInfo (QList<QVariant> list)
{
	name = list[0].toString();
	type = typeOf(list[1].toString());
	color = list[2].value<QColor>();
}

Player::PlayerInfo::operator QList<QVariant>() const
{
	QList<QVariant> res;
	res.append(name);
	res.append(typeString(type));
	return res;
}
#include "player.hpp"
#include "HumanPlayer.hpp"
#include "AIPlayers.hpp"

#include <QListIterator>

const int Player::playerTypes = 3;
const QString Player::playerTypeNames[Player::playerTypes] =
{
	"Human",
	"Easy CPU",
	"Medium CPU",
};

Player::Player(QString name, QColor color, int type, QObject* parent)
	: QObject(parent), typeVal(type), nameVal(name), colorVal(color), 
		pointsVal(0)
{
}

int Player::points() const
{
	return pointsVal;
}

int Player::type() const
{
	return typeVal;
}

QString Player::name() const
{
	return nameVal;
}

QColor Player::color() const
{
	return colorVal;
}

Player::PlayerInfo Player::playerInfo() const
{
	return PlayerInfo(nameVal, typeVal, colorVal);
}

template <class PlayerT>
Player* constructorTemplate(const Player::PlayerInfo& info, QObject* parent = 0)
{
	return new PlayerT(info.name, info.color, parent);
}

typedef Player* (*playerConstructor)(const Player::PlayerInfo& info, QObject* parent);

static int constructors = Player::playerTypes;
static playerConstructor constructor[Player::playerTypes] =
{
	constructorTemplate<HumanPlayer>,
	constructorTemplate<AI1Player>,
	constructorTemplate<AI2Player>,
};

Player* Player::fromPlayerInfo(const PlayerInfo& info, QObject* parent)
{
	return constructor[info.type % constructors](info, parent);
}

Player::PlayerInfo::PlayerInfo ()
{
}

Player::PlayerInfo::PlayerInfo (QString name, int type, QColor color)
	: name(name), type(type), color(color)
{
}

Player::PlayerInfo::PlayerInfo (const QList<QVariant> list)
{
	name = list[0].toString();
	type = list[1].toInt();
	color = list[2].value<QColor>();
}

Player::PlayerInfo::operator QVariant() const
{
	QList<QVariant> res;
	res.append(name);
	res.append(type);
	res.append(color);
	return res;
}

QList<Player*> Player::createPlayers(const QList<Player::PlayerInfo> ls, QObject* parent)
{
	QList<Player*> res;
	QListIterator<Player::PlayerInfo> iterator(ls);
	while (iterator.hasNext())
		res.append(fromPlayerInfo(iterator.next(), parent));
	return res;
}
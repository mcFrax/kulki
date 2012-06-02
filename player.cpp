#include "player.hpp"
#include "HumanPlayer.hpp"
#include "AIPlayers.hpp"

#include <QListIterator>

int Player::playerTypes = 3;
const QString Player::playerTypeNames[Player::playerTypes]
{
	"Human",
	"Easy CPU",
	"Medium CPU",
}

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
Player* constructor(const PlayerInfo& info)
{
	return new PlayerT(info.name, info.color);
}

typedef Player* (*playerConstructor)(const PlayerInfo& info);

static int constructors = Player::playerTypes;
static playerConstructor constructor[Player::playerTypes] =
{
	constructor<HumanPlayer>,
	constructor<AI1Player>,
	constructor<AI2Player>,
};

Player* Player::fromPlayerInfo(const PlayerInfo& info)
{
	return constructor[info.type % constructors](info);
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

Player::PlayerInfo::operator QList<QVariant>() const
{
	QList<QVariant> res;
	res.append(name);
	res.append(type);
	res.append(color);
	return res;
}

QList<Player*> Player::createPlayers(const QList<QVariant> ls)
{
	QList<Player*> res;
	QListIterator<QVariant> iterator(ls);
	while (iterator.hasNext())
		res.append(fromPlayerInfo(iterator.next().toList()));
	return res;
}

QList<QVariant> Player::toQVariantList(const QList<Player*>& ls)
{
	QList<QVariant> res;
	QListIterator<Player*> iterator(ls);
	while (iterator.hasNext())
		res.append(iterator.next()->playerInfo());
	return res;
}
#include "settings.hpp"
#include "player.hpp"

#include <QApplication>
#include <QVariant>
#include <QList>

static QSettings* settingsPtr = 0;

static inline void ensureContains(QString key, QVariant defValue)
{
	if (!settingsPtr->contains(key))
			settingsPtr->setValue(key, defValue);
}

static QList<QVariant> defPlayers()
{
	QList<QVariant> res;
	res.append(QVariant(Player::PlayerInfo("Gracz", Player::human, "#0000FF")));
	res.append(QVariant(Player::PlayerInfo("CPU", Player::cpu1, "#FF0000")));
	return res;
}

static QList<QVariant> defBallTypeSettings()
{
	//prawdopodobienstwo w promilach
	QList<QVariant> res;
	res.append(33); //jokers
	res.append(33); //chameleons
	res.append(33); //doubles
	res.append(33); //skulls
	res.append(33); //hourglasses
	return res;
}

static void ensureContains()
{	
	settingsPtr->beginGroup("square");
		ensureContains("size", 64);
	settingsPtr->endGroup();
	
	settingsPtr->beginGroup("board");
		ensureContains("margin", 10);
		ensureContains("ballPlacingDelay", 55);
	settingsPtr->endGroup();
	
	settingsPtr->beginGroup("ball");
		ensureContains("margin", 3.0);
		ensureContains("appearTime", 500);
		ensureContains("fallingTimeFactor", 480);
		ensureContains("explosionFactor", 2);
		ensureContains("explosionTime", 500);
		ensureContains("arcTime", 420);
		ensureContains("fallingDelay", 350);
		ensureContains("appearDelay", 480);
	settingsPtr->endGroup();
	
	settingsPtr->beginGroup("highlight");
		ensureContains("opacityFactor", 0.8);
		ensureContains("widthFactor", 0.6);
	settingsPtr->endGroup();
	
	settingsPtr->beginGroup("game settings");
		ensureContains("width", 8);
		ensureContains("height", 8);
		ensureContains("colors", 5);
		ensureContains("rowLength", 3);
		ensureContains("turnLimit", 0);
		ensureContains("players", defPlayers());
		ensureContains("ballTypeSettings", defBallTypeSettings());
		ensureContains("roundLimit", 0);
	settingsPtr->endGroup();
	
	settingsPtr->beginGroup("ai");
		ensureContains("moveDelay", 1500);
	settingsPtr->endGroup();
}

QSettings* settings()
{
	if (!settingsPtr){
		settingsPtr = new QSettings(QApplication::instance());
		ensureContains();
	}
	return settingsPtr;
}

void resetDeafaultSettings()
{
	settings()->clear();
	ensureContains();
}
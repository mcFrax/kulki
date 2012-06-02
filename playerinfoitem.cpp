#include "playerinfoitem.hpp"
#include "playericonitem.hpp"
#include "player.hpp"
#include "board.hpp"

#include <QFont>
#include <QBrush>
#include <QPropertyAnimation>

#include <cmath>

#include "debugtools.hpp"

PlayerInfoItem::PlayerInfoItem(Player* player, Board* board, qreal x, qreal y, QGraphicsItem* parentItem)
	: QGraphicsRectItem(parentItem), playerVal(player), boardVal(board), 
		visiblePointsVal(player->points()), anim(0)
{
	#warning about hardcoded values
	setBrush(Qt::white);
	setPos(x, y);
	iconItem = new PlayerIconItem(player->color(), this);
	iconItem->setPos(3, 3);
	
	QFont font;
	font.setPixelSize(24);
	
	nameItem = new QGraphicsSimpleTextItem(player->name(), this);
	nameItem->setPos(40, 6);
	nameItem->setBrush(Qt::white);
	nameItem->setFont(font);
	
	pointsItem = new QGraphicsSimpleTextItem(pointsString(player->points()), this);
	pointsItem->setPos(6, 40);
	pointsItem->setBrush(Qt::white);
	pointsItem->setFont(font);
	
	
	connect(board, SIGNAL(playerTurn(Player*)), this, SLOT(playerTurn(Player*)));
	connect(board, SIGNAL(playerMoved(Player*)), this, SLOT(playerMoved(Player*)));
	connect(board, SIGNAL(playerMoveEnded(Player*, uint)), this, SLOT(playerMoveEnded(Player*, uint)));
	connect(board, SIGNAL(pointsEarned(Player*, uint)), this, SLOT(pointsEarned(Player*, uint)));
}

PlayerInfoItem::~PlayerInfoItem()
{
	if (anim)
		delete anim;
}

uint PlayerInfoItem::visiblePoints()
{
	return visiblePointsVal;
}

void PlayerInfoItem::setVisiblePoints(uint val)
{
	visiblePointsVal = val;
	pointsItem->setText(pointsString(uint(val)));
}

inline QString PlayerInfoItem::pointsString(uint points)
{
	//dosyc brzydkie, ale krotkie i dziala
	const QString& n = QString::number(points);
	QString pref("000000");
	pref.chop(n.size());
	return pref+n;
}

Player* PlayerInfoItem::player()
{
	return playerVal;
}

void PlayerInfoItem::playerTurn(Player* p)
{
	if (p != playerVal) return;
	iconItem->blink(1);
	iconItem->activate();
}

void PlayerInfoItem::playerMoved(Player* p)
{
	if (p != playerVal) return;
	iconItem->noThrobber();
}

void PlayerInfoItem::playerMoveEnded(Player* p, uint total)
{
	if (p != playerVal) return;
	iconItem->deactivate();
}

void PlayerInfoItem::pointsEarned(Player* p, uint points)
{
	if (p != playerVal) return;
	
	if (!anim)
		anim = new QPropertyAnimation(this, "visiblePoints");
	anim->stop();
	anim->setStartValue(visiblePointsVal);
	anim->setEndValue(playerVal->points());
	anim->setDuration(sqrt(playerVal->points() - visiblePointsVal)*25);
	anim->start();
}

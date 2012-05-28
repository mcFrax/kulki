#pragma once

#include <QObject>
#include <QGraphicsRectItem>

class Board;
class Player;
class PlayerIconItem;
class QGraphicsSimpleTextItem;

class PlayerInfoItem : public QObject, public QGraphicsRectItem
{
	Q_OBJECT
	private:
		Player* playerVal;
		Board* boardVal;
		PlayerIconItem* iconItem;
		QGraphicsSimpleTextItem* nameItem;
		QGraphicsSimpleTextItem* pointsItem;
		static QString pointsString(uint points);
	private slots:
		void playerTurn(Player*);
		void playerMoved(Player*);
		void playerMoveEnded(Player*, uint total);
		void pointsEarned(Player*, uint points);
	public:
		PlayerInfoItem(Player*, Board*, qreal x, qreal y, QGraphicsItem* parentItem = 0);
		Player* player();
};
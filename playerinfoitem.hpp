#pragma once

#include <QObject>
#include <QGraphicsRectItem>

class Board;
class Player;
class PlayerIconItem;
class QGraphicsSimpleTextItem;
class QPropertyAnimation;

class PlayerInfoItem : public QObject, public QGraphicsRectItem
{
	Q_OBJECT
	Q_PROPERTY(qreal visiblePoints READ visiblePoints WRITE setVisiblePoints)
	private:
		Player* playerVal;
		Board* boardVal;
		qreal visiblePointsVal;
		PlayerIconItem* iconItem;
		QGraphicsSimpleTextItem* nameItem;
		QGraphicsSimpleTextItem* pointsItem;
		QPropertyAnimation* anim;
		uint visiblePoints();
		void setVisiblePoints(uint);
		static QString pointsString(uint points);
	private slots:
		void playerTurn(Player*);
		void playerMoved(Player*);
		void playerMoveEnded(Player*, uint total);
		void pointsEarned(Player*, uint points);
	public:
		PlayerInfoItem(Player*, Board*, qreal x, qreal y, QGraphicsItem* parentItem = 0);
		~PlayerInfoItem();
		Player* player();
		
		static int width();
		static int height();
};
#pragma once

#include <QColor>
#include <QObject>
#include <QBrush>
#include <QGraphicsEllipseItem>

class QPropertyAnimation;

class PlayerIconItem : public QObject, public QGraphicsEllipseItem
{
	Q_OBJECT
	Q_PROPERTY(qreal scale READ scale WRITE setScale)
	Q_PROPERTY(qreal colorness READ colorness WRITE setColorness)
	private:
		QColor color;
		QPropertyAnimation* throbberAnim;
		qreal colornessVal;
		qreal colorness();
		void setColorness(qreal);
	public:
		PlayerIconItem(QColor color, QGraphicsItem* parentItem = 0);
		~PlayerIconItem();
		void blink();
		void activate();
		void deactivate();
		void throbber();
};
#pragma once

#include <QGraphicsSimpleTextItem>

class Board;

class PointsEarnedItem : public QObject, public QGraphicsSimpleTextItem
{
	Q_OBJECT
	Q_PROPERTY (qreal pos READ pos WRITE setPos)
	Q_PROPERTY (qreal size READ size WRITE setSize)
	private:
		qreal x;
		qreal y;
		qreal posVal;
		qreal pos();
		void setPos(qreal val);
		qreal sizeVal;
		qreal size();
		void setSize(qreal val);
	public:
		PointsEarnedItem(Board*, qreal x, qreal y, int points);
};
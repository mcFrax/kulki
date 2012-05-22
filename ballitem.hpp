#pragma once
#include <QGraphicsEllipseItem>

class Square;

class BallItem : protected QGraphicsEllipseItem
{
	protected:
		static const qreal xmargin;
		static const qreal ymargin;
	public:
		BallItem(const QColor&);
		BallItem(const QColor&, Square*, qreal xoffset = 0, qreal yoffset = 0);
		void placeOnSquare(Square*, qreal xpos = 0, qreal ypos = 0);
		void setPosition(qreal x, qreal y);
		QBrush brush() const;
		void setBrush(const QBrush&);
};
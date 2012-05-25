#pragma once
#include <QGraphicsEllipseItem>

class Square;

class BallItem : public QObject, protected QGraphicsEllipseItem
{
	Q_OBJECT
	Q_PROPERTY(QRectF rect READ rect WRITE setRect)
	protected:
		static const qreal xmargin;
		static const qreal ymargin;
		void animate(qreal yoffset, int animDelay = 0);
	public:
		//~ BallItem(const QColor&);
		BallItem(const QColor&, Square*, qreal yoffset = 0, int animDelay = 0);
		void placeOnSquare(Square*, qreal ypos = 0, int animDelay = 0);
		QBrush brush() const;
		void setBrush(const QBrush&);
		void explode();
};
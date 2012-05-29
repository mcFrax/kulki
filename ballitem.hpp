#pragma once
#include <QGraphicsPixmapItem>

class Square;
class QColor;
class QPixmap;

class BallItem : public QObject, protected QGraphicsPixmapItem
{
	Q_OBJECT
	Q_PROPERTY(QPointF pos READ pos WRITE setPos)
	Q_PROPERTY(qreal scale READ scale WRITE setScale)
	Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
	protected:
		void animateFalling(qreal yoffset, int animDelay = 0);
		void animateAppear(int time = 0);
		void animateArc(QPointF startpoint);
		QGraphicsPixmapItem* glossPixmapItem;
		QGraphicsPixmapItem* specialPixmapItem;
		void scalePixmapItem(const QPixmap&, qreal sizeFactor);
		static QImage glossImage;
		static QImage maskImage;
		static void commonGetPixmap(); //common part of these:
		static QPixmap getPixmap(QColor color);
		static QPixmap getPixmap(QBrush brush);
	public:
		BallItem(const QColor&, Square*, qreal yoffset = 0, int animDelay = 0);
		void placeOnSquare(Square*, qreal ypos = 0, int animDelay = 0);
		void placeOnSquare(Square*, Square* from);
		//~ QBrush brush() const;
		void setBrush(const QBrush&);
		void setSpecialPixmap(const QPixmap&, qreal sizeFactor = 1.0);
		QPixmap specialPixmap();
		void explode();
};
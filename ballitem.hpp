#pragma once
#include <QGraphicsEllipseItem>

class Square;
class QPixmap;
class QGraphicsPixmapItem;

class BallItem : public QObject, protected QGraphicsEllipseItem
{
	Q_OBJECT
	Q_PROPERTY(QPointF pos READ pos WRITE setPos)
	Q_PROPERTY(qreal scale READ scale WRITE setScale)
	protected:
		static const qreal xmargin;
		static const qreal ymargin;
		void animate(qreal yoffset, int animDelay = 0);
		void animateArc(QPointF startpoint);
		QGraphicsPixmapItem* glossPixmapItem;
		QGraphicsPixmapItem* specialPixmapItem;
		void scalePixmapItem(const QPixmap&, qreal sizeFactor);
		static QPixmap* glossPixmap;
	public:
		BallItem(const QColor&, Square*, qreal yoffset = 0, int animDelay = 0);
		void placeOnSquare(Square*, qreal ypos = 0, int animDelay = 0);
		void placeOnSquare(Square*, Square* from);
		QBrush brush() const;
		void setBrush(const QBrush&);
		void setSpecialPixmap(const QPixmap&, qreal sizeFactor = 1.0);
		QPixmap specialPixmap();
		void explode();
		QGraphicsEllipseItem::sceneBoundingRect;
};
#pragma once

#include <QColor>
#include <QObject>
#include <QPixmap>
#include <QGraphicsItem>

class QImage;
class QPainter;
class QStyleOptionGraphicsItem;
class QPropertyAnimation;

class PlayerIconItem : public QObject, public QGraphicsItem
{
	Q_OBJECT
	Q_PROPERTY(qreal scale READ scale WRITE setScale)
	Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
	private:
		QColor color;
		QPropertyAnimation* throbberAnim;
		QPixmap pixmap;
		static QImage glossImage;
		static QImage maskImage;
		static QPixmap getPixmap(QColor color);
		void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget * widget = 0);
	public:
		PlayerIconItem(QColor color, QGraphicsItem* parentItem = 0);
		~PlayerIconItem();
		QRectF boundingRect() const;
	public slots:
		void blink();
		void blink(bool throbber);
		void activate();
		void deactivate();
		void throbber();
		void noThrobber();
};
#include <QImage>
#include <QPainter>
#include <QColor>


//!Czesc wspolna obu wersji (z QColor i QBrush)
static void glossyColorShape(QPainter& painter, const QImage& mask, const QImage& gloss)
{
	painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
	painter.drawImage(QPointF(0, 0), mask);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	painter.drawImage(QPointF(0, 0), gloss);
}

QImage glossyColorShape(QColor color, const QImage& mask, const QImage& gloss)
{
	QImage result = QImage(mask.width(), mask.height(), 
			QImage::Format_ARGB32_Premultiplied);
	result.fill(color);
	QPainter painter(&result);
	glossyColorShape(painter, mask, gloss);
	return result;
}

QImage glossyColorShape(QBrush brush, const QImage& mask, const QImage& gloss)
{
	QImage result = QImage(mask.width(), mask.height(), 
			QImage::Format_ARGB32_Premultiplied);
	QRectF r = result.rect();
	QPainter painter(&result);
	painter.fillRect(result.rect(), brush);
	glossyColorShape(painter, mask, gloss);
	return result;
}
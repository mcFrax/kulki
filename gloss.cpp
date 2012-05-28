#include <QPixmap>
#include <QColor>
#include <QImage>
#include <QPaintEngine>

QImage glossyColorShape(QColor color, QImage mask, QImage gloss)
{
	QImage result = QImage(mask.width(), mask.height(), QImage::Format_ARGB32);
	result.fill(color);
	switch (mask.format()) {
		case QImage::Format_Indexed8:
			for (int iy = 0; iy < result.height(); ++iy){
				QRgb* resLine = reinterpret_cast<QRgb*>(result.scanLine(iy));
				uchar* maskLine = mask.scanLine(iy);
				for (int ix = 0; ix < result.width(); ++ix){
					resLine[ix] = qRgba(
							qRed(resLine[ix]),
							qGreen(resLine[ix]),
							qBlue(resLine[ix]),
							maskLine[ix]);
				}
			}
			break;
		case QImage::Format_RGB32:
		case QImage::Format_ARGB32:
			for (int iy = 0; iy < result.height(); ++iy){
				QRgb* resLine = reinterpret_cast<QRgb*>(result.scanLine(iy));
				QRgb* maskLine = reinterpret_cast<QRgb*>(mask.scanLine(iy));
				for (int ix = 0; ix < result.width(); ++ix){
					resLine[ix] = qRgba(
							qRed(resLine[ix]),
							qGreen(resLine[ix]),
							qBlue(resLine[ix]),
							qRed(maskLine[ix]));
				}
			}
			break;
		default:
			//possibly error message
			break;
	}
	
	result.paintEngine()->drawImage(result.rect(), gloss, gloss.rect(), 
			Qt::AutoColor | Qt::NoOpaqueDetection);
	return result;
}
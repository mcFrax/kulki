#pragma once

class QImage;
class QColor;
class QBrush;

QImage glossyColorShape(QColor color, const QImage& mask, const QImage& gloss);
QImage glossyColorShape(QBrush brush, const QImage& mask, const QImage& gloss);
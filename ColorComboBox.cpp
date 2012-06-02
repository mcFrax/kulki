/*
 * Na podstawie przykladu
 * Color Editor Factory
 * 
 * http://doc.qt.nokia.com/4.7-snapshot/itemviews-coloreditorfactory.html
 */
 
#include "ColorComboBox.hpp" 

#include <QColor>
#include <QStringList>
#include <QVariant>

ColorComboBox::ColorComboBox(QColor color, QWidget *widget)
	: QComboBox(widget)
{
	populateList();
	setColor(color);
}

QColor ColorComboBox::color() const
{
	return qVariantValue<QColor>(itemData(currentIndex(), Qt::DecorationRole));
}

void ColorComboBox::setColor(QColor color)
{
	setCurrentIndex(findData(color, int(Qt::DecorationRole)));
}

void ColorComboBox::populateList()
{
	QStringList colorNames = QColor::colorNames();

	for (int i = 0; i < colorNames.size(); ++i) {
		QColor color(colorNames[i]);

		insertItem(i, colorNames[i]);
		setItemData(i, color, Qt::DecorationRole);
	}
}
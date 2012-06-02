/*
 * Na podstawie przykladu
 * Color Editor Factory
 * 
 * http://doc.qt.nokia.com/4.7-snapshot/itemviews-coloreditorfactory.html
 */
 
#pragma once

#include <QComboBox>
#include <QColor>

class ColorComboBox : public QComboBox
{
	Q_OBJECT
	Q_PROPERTY(QColor color READ color WRITE setColor USER true)

	public:
		ColorComboBox(QColor color = QColor(), QWidget *widget = 0);

	public:
		QColor color() const;
		void setColor(QColor c);

	private:
		void populateList();
};
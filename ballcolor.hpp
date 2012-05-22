#pragma once
#include <QColor>

class BallColor {
	private:
		static uint colors;
	protected:
		uint colorNumber;
	public:
		BallColor(uint colorNumber = BallColor::noneNumber);
		bool operator == (const BallColor& colorNumber) const;
		bool operator != (const BallColor& colorNumber) const;
		operator uint() const;
		operator QColor() const;
		
		static const uint noneNumber;
		static const uint jokerNumber;
		static const BallColor none;
		static const BallColor joker;
		static QColor* table;
		static void createTable(uint colors);
		static BallColor random();
		static uint availableColors();
};
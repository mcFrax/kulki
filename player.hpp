#pragma once
#include <QObject>

class Board;

class Player : public QObject
{
	Q_OBJECT
	friend class Board;
	private:
		QString nameVal;
		int pointsVal;
	protected:
		virtual void makeMove(Board*) = 0;
		Player(QString name);
	public:
		virtual bool isHuman() const = 0;
		int points();
		QString name();
};
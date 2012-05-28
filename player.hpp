#pragma once
#include <QObject>
#include <QList>
#include <QVariant>
#include <QColor>

class Board;

class Player : public QObject
{
	Q_OBJECT
	friend class Board;
	friend class BoardImplementation;
	public:
		struct PlayerInfo
		{
			enum Type
			{
				human,
				cpu,
				unknown
			};
			QString name;
			Type type;
			QColor color;
			PlayerInfo ();
			PlayerInfo (QString name, QString type, QColor color);
			PlayerInfo (QList<QVariant>);
			operator QList<QVariant>() const;
		};
	private:
		QString nameVal;
		QColor colorVal;
		int pointsVal;
	protected:
		virtual void makeMove(Board*) = 0;
		Player(QString name, QColor color);
	public:
		virtual bool isHuman() const = 0;
		int points();
		QColor color();
		QString name();
		//~ virtual PlayerInfo playerInfo() = 0;
		//~ Player* fromPlayerInfo(const PlayerInfo& info);
};
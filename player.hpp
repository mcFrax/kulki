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
			QString name;
			int type;
			QColor color;
			PlayerInfo ();
			PlayerInfo (QString name, int type, QColor color);
			PlayerInfo (const QList<QVariant>);
			operator QVariant() const;
		};
		enum { human = 0, cpu1 = 1, cpu2 = 2 };
	private:
		const int typeVal;
		QString nameVal;
		QColor colorVal;
		int pointsVal;
	protected:
		virtual void makeMove(Board*) = 0;
		Player(QString name, QColor color, int type, QObject* parent = 0);
	public:
		virtual bool isHuman() const = 0;
		int points() const;
		int type() const;
		QColor color() const;
		QString name() const;
		PlayerInfo playerInfo()  const;
		
		static Player* fromPlayerInfo(const PlayerInfo& info, QObject* parent = 0);
		static QList<Player*> createPlayers(const QList<Player::PlayerInfo>, QObject* parent = 0);
		
		static const int playerTypes;
		static const QString playerTypeNames[];
};
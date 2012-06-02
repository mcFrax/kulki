#pragma once

#include <queue>

#include "player.hpp"

class QColor;

class AIPlayer : public Player
{
	Q_OBJECT
	protected:
		std::queue<Board*> boards;
		void makeMove(Board*);
		AIPlayer(QString name, QColor color, int type, QObject* parent = 0);
	protected slots:
		virtual void makeRealMove() = 0;
	public:
		bool isHuman() const;
};

class AI1Player : public AIPlayer
{
	private:
		void makeRealMove();
	public:
		AI1Player(QString name, QColor color, QObject* parent = 0);
};


class AI2Player : public AIPlayer
{
	private:
		void makeRealMove();
	public:
		AI2Player(QString name, QColor color, QObject* parent = 0);
};
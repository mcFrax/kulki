#pragma once
#include <QGraphicsRectItem>
#include <QList>

#include <set>

#include "array2.hpp"
#include "player.hpp"

class QPoint;
class Square;
class BallColor;
class QAbstractAnimation;
class QPlayerInfoItem;

class Board : public QObject, public QGraphicsRectItem
{
	Q_OBJECT
	public:
		enum State
		{
			waitingForPlayer, //nothing
			waitingForMove,
			animatingMove,
			locked, //game ended
		};
		struct GameSetup
		{
			int width;  //
			int height; //in squares
			uint colors;
			int rowLength;
			QList<Player::PlayerInfo> players;
			QList<uint> ballTypeSettings;
			uint roundLimit;
			
			GameSetup();
			void setAsDefault();
		};
		class BoardInfo
		{
			private:
				Square** array;
				int arrayWidth;
				int arrayHeight;
			public:
				BoardInfo(Square** array, int width, int height);
				BallColor operator () (int x, int y) const;
				int width() const;
				int height() const;
				bool contains(int x, int y) const;
		};
	protected:
		enum InternalState
		{
			normal,
			preparing,
			falling
		};
		const GameSetup setup;
		State state;
		InternalState internalState;
		Player* curPlayer;
		uint turnLimit;
		uint turnNumber;
		QList<Player*> players;
		Array2<Square*> squares;
		std::set<std::pair<Square*, Square*> > legalMoves;
		std::set<QObject*> currentAnimations;
	protected:
		Board(const GameSetup&, QGraphicsItem * parent = 0);
		void setState(State);
		virtual void check() = 0;
		virtual void refill(int animDelay = 0) = 0;
		static qreal margin();
	public:
		static Board* newBoard(const GameSetup&, QGraphicsItem * parent = 0);
		~Board();
		
		Player* currentPlayer();
		
		const GameSetup& gameSetup();
		
		State getState();
		uint turn();
		
		bool inBoard(int x, int y) const;
		
		bool isLegal(Square*, Square*) const;
		bool isLegal(QPoint, QPoint) const;
		
		virtual bool move(Square*, Square*) = 0;
		bool move(QPoint, QPoint);
		
		void registerAnimation(QAbstractAnimation* anim);
	signals:
		void playerTurn(Player*);
		void playerMoved(Player*);
		void playerMoveEnded(Player*, uint total); //or fallingEnded
		void pointsEarned(Player*, uint points);
		void gameEnded();
		void stateChanged(Board::State);
	public slots:
		void animationEnded();
};

std::ostream& operator << (std::ostream&, Board::State);
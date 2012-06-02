#pragma once
#include <QGraphicsRectItem>
#include <QList>

#include <set>

#include "array2.hpp"

class QPoint;
class Square;
class Player;
class BallColor;
class QAbstractAnimation;

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
			uint width;  //
			uint height; //in squares
			uint colors;
			uint rowLength;
			QList<Player*> players;
			//list_of_players
			//time_limit
			//bonus chances
			//points factor
			//etc, etc
		};
		class BoardInfo
		{
			private:
				Square** array;
				uint arrayWidth;
				uint arrayHeight;
			public:
				BoardInfo(Square** array, uint width, uint height);
				BallColor operator () (uint x, uint y) const;
				uint width() const;
				uint height() const;
				bool contains(int x, int y) const;
		};
	protected:
		enum InternalState
		{
			normal,
			falling
		};
		const GameSetup setup;
		State state;
		InternalState internalState;
		Player* curPlayer;
		Player* nextPlayer;
		Array2<Square*> squares;
		std::set<std::pair<Square*, Square*> > legalMoves;
		std::set<QObject*> currentAnimations;
	protected:
		Board(const GameSetup&, QGraphicsItem * parent = 0);
		void setState(State);
		virtual void check() = 0;
		virtual void refill(int animDelay = 0) = 0;
	public:
		static Board* newBoard(const GameSetup&, QGraphicsItem * parent = 0);
		
		Player* currentPlayer();
		Player* getNextPlayer();
		
		const GameSetup& gameSetup();
		
		State getState();
		
		bool inBoard(int x, int y);
		
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
		virtual void setCurrentPlayer(Player*) = 0;
		void animationEnded();
};

std::ostream& operator << (std::ostream&, Board::State);
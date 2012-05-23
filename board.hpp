#pragma once
#include <QGraphicsScene>
#include <set>

class Square;
class Player;
class BallColor;

class Board : public QGraphicsScene
{
	Q_OBJECT
	friend class Square;
	public:
		enum State {
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
		const GameSetup setup;
		State state;
		Player* curPlayer;
		Player* nextPlayer;
		std::set<std::pair<Square*, Square*> > legalMoves;
		std::set<QObject*> currentAnimations;
	protected:
		Board(const GameSetup&, QObject * parent = 0);
		void setState(State);
		virtual void check() = 0;
	public:
		static Board* newBoard(const GameSetup&, QObject * parent = 0);
		
		Player* currentPlayer();
		Player* getNextPlayer();
		
		const GameSetup& gameSetup();
		
		State getState();
		
		bool inBoard(int x, int y);
		
		bool isLegal(Square*, Square*) const;
		
		virtual bool move(Square*, Square*) = 0;
		
		void registerAnimation(QObject* anim);
	signals:
		//~ void playerMoved(Player*);
		void playerMoveEnded(Player*, uint total); //or fallingEnded
		//~ void pointsEarned(Player*, uint points);
		//~ void gameEnded();
		void stateChanged(Board::State);
	public slots:
		virtual void setCurrentPlayer(Player*) = 0;
		void animationEnded();
};
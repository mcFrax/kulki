#pragma once
#include <QGraphicsScene>
#include <set>

class Square;
class Player;
class BallColor;

class Board : public QGraphicsScene
{
	Q_OBJECT
	public:
		enum State {
			normal, //nothing
			playerMove,
			falling,
			checking,
			cleaning,
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
		std::set<std::pair<Square*, Square*> > legalMoves;
	protected:
		Board(const GameSetup&, QObject * parent = 0);
	public:
		static Board* newBoard(const GameSetup&, QObject * parent = 0);
		
		Player* currentPlayer();
		
		const GameSetup& gameSetup();
		
		State getState();
		
		bool inBoard(int x, int y);
		
		bool isLegal(Square*, Square*) const;
	signals:
		void playerMoved(Player*);
		void playerMoveEnded(Player*, uint total); //or fallingEnded
		void pointsEarned(Player*, uint points);
		void gameEnded();
	public slots:
		virtual void setCurrentPlayer(Player*) = 0;
};
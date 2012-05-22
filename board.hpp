#pragma once
#include <QGraphicsScene>

class Square;
class Player;

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
	protected:
		const GameSetup setup;
		State state;
		Player* curPlayer;
	protected:
		Board(const GameSetup&, QObject * parent = 0);
	public:
		static Board* newBoard(const GameSetup&, QObject * parent = 0);
		
		Player* currentPlayer();
		
		const GameSetup& gameSetup();
		
		State getState();
		
		bool inBoard(int x, int y);
	signals:
		void playerMoved(Player*);
		void playerMoveEnded(Player*, uint total); //or fallingEnded
		void pointsEarned(Player*, uint points);
		void gameEnded();
	public slots:
		virtual void setCurrentPlayer(Player*) = 0;
};
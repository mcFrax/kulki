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
	public:
		Board(const GameSetup&, QObject * parent = 0);
		
		Player* currentPlayer();
		
		const GameSetup& gameSetup();
		
		State getState();
		
	signals:
		void playerMoved(Player*);
		void playerMoveEnded(Player*, uint total); //or fallingEnded
		void pointsEarned(Player*, uint points);
		void gameEnded();
	public slots:
		virtual void setCurrentPlayer(Player*) = 0;
};
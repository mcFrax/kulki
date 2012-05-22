#pragma once
#include <QGraphicsScene>
#include <set>

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
		Square** squares;
		Square* square(uint x, uint y);
		std::set<std::pair<Square*, Square*> > legalMoves;
		State state;
		Player* curPlayer;
	protected:
		void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);
		void check();
		void computeLegalMoves(int, int);
		void countSame();
		void computeLegalMoves();
	private slots:
		void squarePressed(Square*, Square*);
	public:
		Board(const GameSetup&, QObject * parent = 0);
		~Board();
		
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
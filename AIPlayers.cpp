#include "AIPlayers.hpp"
#include "board.hpp"

#include <cstdlib>
#include <QTimer>

#include "settings.hpp"

AIPlayer::AIPlayer(QString name, QColor color, int type, QObject* parent)
	: Player(name, color, type, parent)
{
}

void AIPlayer::makeMove(Board* board)
{
	boards.push(board);
	QTimer::singleShot(settings()->value("ai/moveDelay").toInt(), 
			this, SLOT(makeRealMove()));
}

bool AIPlayer::isHuman() const
{
	return 0;
}


AI1Player::AI1Player(QString name, QColor color, QObject* parent)
	: AIPlayer(name, color, 1, parent)
{
}

void AI1Player::makeRealMove()
{
	if (boards.empty()) return;
	Board* board = boards.front();
	boards.pop();
	while (1){
		int ix = rand()%board->gameSetup().width;
		int iy = rand()%board->gameSetup().height;
		bool hor = rand()%2;
		if (board->isLegal(QPoint(ix, iy), 
				QPoint(ix+int(hor), iy+int(!hor)))){
			board->move(QPoint(ix, iy), 
				QPoint(ix+int(hor), iy+int(!hor)));
			return;
		}
	}
}

AI2Player::AI2Player(QString name, QColor color, QObject* parent)
	: AIPlayer(name, color, 2, parent)
{
}

void AI2Player::makeRealMove()
{
	if (boards.empty()) return;
	Board* board = boards.front();
	boards.pop();
	while (1){
		for (int iy = board->gameSetup().height-1; iy >= 0; --iy){
			for (int k = 0; k < (board->gameSetup().width+1)/2; ++k){
				int ix = board->gameSetup().width/2 + k;
				{
					if (board->isLegal(QPoint(ix, iy), QPoint(ix+1, iy)) && (rand()%2)){
						board->move(QPoint(ix, iy), QPoint(ix+1, iy));
						return;
					}
					if (board->isLegal(QPoint(ix, iy), QPoint(ix, iy+1)) && (rand()%2)){
						board->move(QPoint(ix, iy), QPoint(ix, iy+1));
						return;
					}
				}
				
				ix = board->gameSetup().width/2 - k;
				{
					if (board->isLegal(QPoint(ix, iy), QPoint(ix+1, iy)) && (rand()%2)){
						board->move(QPoint(ix, iy), QPoint(ix+1, iy));
						return;
					}
					if (board->isLegal(QPoint(ix, iy), QPoint(ix, iy+1)) && (rand()%2)){
						board->move(QPoint(ix, iy), QPoint(ix, iy+1));
						return;
					}
				}
			}
		}
	}
}
#include "aiplayer.hpp"
#include "board.hpp"

#include <cstdlib>
#include <QTimer>

#include "settings.hpp"

void makeMove(Board*);
AIPlayer::AIPlayer(QColor color)
	: Player("AI", color)
{
}

void AIPlayer::makeMove(Board* board)
{
	boards.push(board);
	QTimer::singleShot(settings()->value("ai/moveDelay").toInt(), 
			this, SLOT(makeRealMove()));
}


void AIPlayer::makeRealMove()
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

bool AIPlayer::isHuman() const
{
	return 0;
}
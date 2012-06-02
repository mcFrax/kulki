#include <QGraphicsView>
#include <QGraphicsScene>
#include <QBrush>
#include <QColor>
#include <QPixmap>

#include "main_window.hpp"
#include "player.hpp"
#include "board.hpp"
#include "playerinfoitem.hpp"

#include "settings.hpp"

using namespace std;

MainWindow::MainWindow()
{
	
	Board::GameSetup gameSetup;
	settings()->beginGroup("game settings/");
		gameSetup.width =  settings()->value("width").toUInt();
		gameSetup.height = settings()->value("height").toUInt();
		gameSetup.colors = settings()->value("colors").toUInt();
		gameSetup.rowLength = settings()->value("rowLength").toUInt();
		players = Player::createPlayers(settings()->value("players").toList());
		gameSetup.players = players;
	settings()->endGroup();
	
	scene = new QGraphicsScene(this);
	scene->setBackgroundBrush(QBrush(QPixmap(":Background.jpg")));
	board = Board::newBoard(gameSetup);
	scene->addItem(board);
	
	board->setCurrentPlayer(players[0]);
	connect(board, SIGNAL(playerMoveEnded(Player*, uint)), this, SLOT(nextPlayer(Player*)));
	
	for (int i = 0; i < players.size(); ++i){
		PlayerInfoItem* piitem = new PlayerInfoItem(players[i], board, board->rect().width(), i*74);
		scene->addItem(piitem);
	}
	
	scene->setSceneRect(scene->sceneRect()); //ustawia biezacy na sztywno

	graphicsView = new QGraphicsView(scene, this);
	graphicsView->setMouseTracking(1);

	setCentralWidget(graphicsView);
}

void MainWindow::nextPlayer(Player* p)
{
	for (int i = 0; i < players.size(); ++i){
		if (p == players[i]){
			if (i == players.size()-1)
				board->setCurrentPlayer(players[0]);
			else
				board->setCurrentPlayer(players[i+1]);
			return;
		}
	}
}
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QBrush>
#include <QColor>
#include <QPixmap>

#include "main_window.hpp"
#include "humanplayer.hpp"
#include "aiplayer.hpp"
#include "board.hpp"
#include "playerinfoitem.hpp"

#include "settings.hpp"

using namespace std;

MainWindow::MainWindow()
{
	Board::GameSetup gameSetup;
	gameSetup.width =  8;
	gameSetup.height = 8;
	gameSetup.colors = 12;
	gameSetup.rowLength = 3;
	
	scene = new QGraphicsScene(this);
	scene->setBackgroundBrush(QBrush(QPixmap(":Background.jpg")));
	board = Board::newBoard(gameSetup);
	scene->addItem(board);
	
	human = new HumanPlayer("Matou", Qt::blue);
	ai = new AIPlayer(Qt::red);
	board->setCurrentPlayer(human);
	connect(board, SIGNAL(playerMoveEnded(Player*, uint)), this, SLOT(nextPlayer(Player*)));
	
	PlayerInfoItem* piitem1 = new PlayerInfoItem(human, board, board->rect().width(), 0);
	scene->addItem(piitem1);
	
	PlayerInfoItem* piitem2 = new PlayerInfoItem(ai, board, board->rect().width(), 74);
	scene->addItem(piitem2);
	
	scene->setSceneRect(scene->sceneRect()); //ustawia biezacy na sztywno

	graphicsView = new QGraphicsView(scene, this);
	graphicsView->setMouseTracking(1);

	setCentralWidget(graphicsView);
}

void MainWindow::nextPlayer(Player* p)
{
	if (p == human)
		board->setCurrentPlayer(ai);
	else
		board->setCurrentPlayer(human);
}
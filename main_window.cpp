#include <QGraphicsView>

#include "main_window.hpp"
#include "humanplayer.hpp"
#include "aiplayer.hpp"
#include "board.hpp"

using namespace std;

MainWindow::MainWindow()
{
	Board::GameSetup gameSetup;
	gameSetup.width = 10;
	gameSetup.height = 8;
	gameSetup.colors = 5;
	gameSetup.rowLength = 3;
	
	scene = Board::newBoard(gameSetup, this);
	
	human = new HumanPlayer("Matou");
	ai = new AIPlayer();
	scene->setCurrentPlayer(human);
	connect(scene, SIGNAL(playerMoveEnded(Player*, uint)), this, SLOT(nextPlayer(Player*)));

	graphicsView = new QGraphicsView(scene, this);
	graphicsView->setMouseTracking(1);

	setCentralWidget(graphicsView);
}

void MainWindow::nextPlayer(Player* p)
{
	if (p == human)
		scene->setCurrentPlayer(ai);
	else
		scene->setCurrentPlayer(human);
}
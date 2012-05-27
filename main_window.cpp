#include <QGraphicsView>
#include <QGraphicsScene>

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
	
	scene = new QGraphicsScene(this);
	board = Board::newBoard(gameSetup);
	scene->addItem(board);
	
	human = new HumanPlayer("Matou");
	ai = new AIPlayer();
	board->setCurrentPlayer(human);
	connect(board, SIGNAL(playerMoveEnded(Player*, uint)), this, SLOT(nextPlayer(Player*)));

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
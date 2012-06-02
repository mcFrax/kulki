#include <QGraphicsView>
#include <QGraphicsScene>
#include <QBrush>
#include <QColor>
#include <QPixmap>
#include <QApplication>

#include "main_window.hpp"
#include "player.hpp"
#include "board.hpp"
#include "playerinfoitem.hpp"
#include "NewGameDialog.hpp"

#include "settings.hpp"
#include "debugtools.hpp"

using namespace std;

MainWindow::MainWindow()
{
	scene = new QGraphicsScene(this);

	graphicsView = new QGraphicsView(scene, this);
	graphicsView->setMouseTracking(1);

	setCentralWidget(graphicsView);
	
	NewGameDialog dialog(Board::GameSetup(), this);
	
	dialog.exec();
	
	scene->setBackgroundBrush(QBrush(QPixmap(":Background.jpg")));
	board = Board::newBoard(dialog.setup());
	scene->addItem(board);
	
	for (int i = 0; i < players.size(); ++i){
		PlayerInfoItem* piitem = new PlayerInfoItem(players[i], board, board->rect().width(), i*74);
		scene->addItem(piitem);
	}
	
	scene->setSceneRect(scene->sceneRect()); //ustawia biezacy na sztywno
}
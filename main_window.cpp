#include <QGraphicsView>
#include <QGraphicsScene>
#include <QBrush>
#include <QColor>
#include <QPixmap>
#include <QMenuBar>

#include "main_window.hpp"
#include "player.hpp"
#include "board.hpp"
#include "playerinfoitem.hpp"
#include "NewGameDialog.hpp"

#include "settings.hpp"
#include "debugtools.hpp"

using namespace std;

MainWindow::MainWindow()
	: graphicsView(0), scene(0), board(0)
{
	scene = new QGraphicsScene(this);
	scene->setBspTreeDepth(9);
	scene->setBackgroundBrush(QBrush(QPixmap(":Background.jpg")));

	graphicsView = new QGraphicsView(scene, this);
	graphicsView->setMouseTracking(1);

	setCentralWidget(graphicsView);
	
	QAction* newGameAction = menuBar()->addAction(tr("Nowa gra"));
	connect(newGameAction, SIGNAL(triggered()), this, SLOT(newGame()));
}


void MainWindow::newGame()
{
	NewGameDialog dialog(Board::GameSetup(), this);
	
	if (dialog.exec() != QDialog::Accepted) return;
	
	if (board) delete board;
	
	board = Board::newBoard(dialog.setup());
	scene->addItem(board);
	
	scene->setSceneRect(QRectF());
	scene->setSceneRect(scene->sceneRect());
	
	QRect r(scene->sceneRect().toRect());
	r.moveTo(graphicsView->geometry().topLeft());
	graphicsView->setGeometry(r);
	
	r.adjust(0, 0, 50, 50); // <- to powinno byc jakos sensowniej
	r.moveTo(geometry().topLeft());
	setGeometry(r);
}
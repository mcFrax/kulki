#include <QGraphicsView>
#include <QGraphicsScene>
#include <QBrush>
#include <QColor>
#include <QPixmap>
#include <QMenuBar>
#include <QPainter>
#include <QResizeEvent>

#include "main_window.hpp"
#include "player.hpp"
#include "board.hpp"
#include "playerinfoitem.hpp"
#include "NewGameDialog.hpp"
#include "HighScores.hpp"

#include "settings.hpp"
#include "debugtools.hpp"

using namespace std;

class GameView : public QGraphicsView
{
	protected:
		void resizeEvent(QResizeEvent * event)
		{
			if (scene() && MainWindow::instance()->board())
				fitInView(MainWindow::instance()->board(), Qt::KeepAspectRatio);
		}
	public:
		GameView(QGraphicsScene* scene = 0, QWidget* parent = 0)
			: QGraphicsView(scene, parent)
		{
			setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
			setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
			//~ if (scene && MainWindow::instance()->board())
				//~ fitInView(MainWindow::instance()->board(), Qt::KeepAspectRatio);
		}
};

MainWindow* MainWindow::instanceVal = 0;

MainWindow::MainWindow()
	: graphicsView(0), scene(0), boardVal(0)
{
	scene = new QGraphicsScene(this);
	scene->setBspTreeDepth(9);
	scene->setBackgroundBrush(QBrush(QPixmap(":Background.jpg")));

	graphicsView = new GameView(scene, this);
	graphicsView->setMouseTracking(1);
	graphicsView->setRenderHint(QPainter::SmoothPixmapTransform, 
		settings()->value("graphics/SmoothPixmapTransform").toBool());

	setCentralWidget(graphicsView);
	
	QAction* newGameAction = menuBar()->addAction(tr("Nowa gra"));
	connect(newGameAction, SIGNAL(triggered()), this, SLOT(newGame()));
	
	QAction* highScoresAction = menuBar()->addAction(tr("Najlepsze wyniki"));
	connect(highScoresAction, SIGNAL(triggered()), this, SLOT(highScores()));
}

MainWindow::~MainWindow()
{
	instanceVal = 0;
}

const Board* MainWindow::board() const
{
	return boardVal;
}

MainWindow* MainWindow::instance()
{
	if (!instanceVal)
		instanceVal = new MainWindow();
	return instanceVal;
}

void MainWindow::newGame()
{
	NewGameDialog dialog(Board::GameSetup(), this);
	
	if (dialog.exec() != QDialog::Accepted) return;
	
	if (boardVal) delete boardVal;
	
	boardVal = Board::newBoard(dialog.setup());
	scene->addItem(boardVal);
	
	//ustawia geometrie sceny wg zawartosci
	//scene->setSceneRect(QRectF(0,0,0,0));
	scene->setSceneRect(QRectF());
	scene->setSceneRect(scene->sceneRect());
	
	graphicsView->fitInView(boardVal, Qt::KeepAspectRatio);
}

void MainWindow::highScores()
{
	showHighScores();
}
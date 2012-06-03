#pragma once

#include <QMainWindow>
#include <QList>

class Board;
class Player;
class QGraphicsView;
class QGraphicsScene;

class MainWindow : public QMainWindow
{
	Q_OBJECT

	protected:
		QGraphicsView *graphicsView;
		QGraphicsScene *scene;
		Board *board;
		static MainWindow* instanceVal;
		MainWindow();
		~MainWindow();
	protected slots:
		void newGame();
		void highScores();
	public:
		static MainWindow* instance();
};
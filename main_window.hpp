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
	protected slots:
		void newGame();
	public:
		MainWindow();
};
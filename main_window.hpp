#pragma once

#include <QMainWindow>

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
		Player* human;
		Player* ai;
	protected slots:
		void nextPlayer(Player*);
	public:
		MainWindow();
};
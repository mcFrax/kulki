#pragma once

#include <QMainWindow>

class Board;
class Player;
class QGraphicsView;

class MainWindow : public QMainWindow
{
	Q_OBJECT

	protected:
		QGraphicsView *graphicsView;
		Board *scene;
		Player* human;
		Player* ai;
	protected slots:
		void nextPlayer(Player*);
	public:
		MainWindow();
};
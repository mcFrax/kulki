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
		QList<Player*> players;
	protected slots:
		void nextPlayer(Player*);
	public:
		MainWindow();
};
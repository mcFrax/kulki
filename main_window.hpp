#pragma once

#include <QList>
#include <QMainWindow>

class Board;
class Player;
class QGraphicsScene;
class GameView;

class MainWindow : public QMainWindow
{
	Q_OBJECT

	protected:
		GameView *graphicsView;
		QGraphicsScene *scene;
		Board *boardVal;
		static MainWindow* instanceVal;
		MainWindow();
		~MainWindow();
	protected slots:
		void newGame();
		void highScores();
	public:
		const Board* board() const;
		static MainWindow* instance();
};
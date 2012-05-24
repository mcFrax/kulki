#pragma once

#include <QMainWindow>

class Board;
class QGraphicsView;

class MainWindow : public QMainWindow
{
	Q_OBJECT

	protected:
		QGraphicsView *graphicsView;
		Board *scene;
	public:
		MainWindow();
};
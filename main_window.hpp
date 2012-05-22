#pragma once

#include <QMainWindow>
#include <QGraphicsView>

class Board;

class MainWindow : public QMainWindow
{
	Q_OBJECT

	protected:
		QGraphicsView *graphicsView;
		Board *scene;
	public:
		MainWindow();
};
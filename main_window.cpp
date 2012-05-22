#include "main_window.hpp"
//~ #include <iostream>
//~ #include <stdexcept>
//~ #include <fstream>
//~ #include <QBrush>
//~ #include <QColor>

#include "board.hpp"

using namespace std;

MainWindow::MainWindow()
{
	#warning hardcoded setup
	Board::GameSetup gameSetup;
	gameSetup.width = 10;
	gameSetup.height = 10;
	gameSetup.colors = 5;
	gameSetup.rowLength = 3;
	
	scene = Board::newBoard(gameSetup, this);
	
	#warning scene->setCurrentPlayer(0);
	scene->setCurrentPlayer(0);

	graphicsView = new QGraphicsView(scene, this);
	graphicsView->setMouseTracking(1);

	setCentralWidget(graphicsView);
}
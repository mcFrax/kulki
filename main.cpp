#include <QApplication>

#include "main_window.hpp"


int main(int argc, char ** argv)
{
	QApplication app(argc, argv);

	MainWindow mainWindow;
	mainWindow.setGeometry(100, 100, 800, 500);
	mainWindow.show();

	return app.exec();
}
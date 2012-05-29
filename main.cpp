#include <QApplication>
#include <QPixmap>
#include <QIcon>

#include "main_window.hpp"
#include "settings.hpp"

int main(int argc, char ** argv)
{
	QApplication app(argc, argv);
	app.setOrganizationName("FraxSoftware");
	app.setApplicationName("Kulki");
	
	app.setWindowIcon(QPixmap(":Icon.png"));

	MainWindow mainWindow;
	mainWindow.show();
	
	return app.exec();
}
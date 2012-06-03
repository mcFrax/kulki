#include <QApplication>
#include <QPixmap>
#include <QIcon>

#include "main_window.hpp"
#include "settings.hpp"

int main(int argc, char ** argv)
{
	srand(time(0));
	
	QApplication app(argc, argv);
	app.setOrganizationName("FraxSoftware");
	app.setApplicationName("Kulki");
	
	app.setWindowIcon(QPixmap(":Icon.png"));

	MainWindow::instance()->show();
	
	return app.exec();
}
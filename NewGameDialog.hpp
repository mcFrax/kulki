#include <QDialog>
#include <QList>

#include "board.hpp"

class QLineEdit;
class PlayerSetupWidget;
class BallTypeSettingsDialog;
class QIntValidator;

class NewGameDialog : public QDialog
{
	Q_OBJECT
	private:
		Board::GameSetup setupVal;
		QLineEdit *widthEdit;
		QLineEdit *heightEdit;
		QLineEdit *colorsEdit;
		QIntValidator* rowLengthValidator;
		QLineEdit *rowLengthEdit;
		QLineEdit *roundLimitEdit;
		QList<PlayerSetupWidget*> players;
		QList<uint> ballSettings;
	private slots:
		void playerWidgetDeleted();
		void showBallTypeSettings();
	public:
		NewGameDialog(const Board::GameSetup& setup = Board::GameSetup(), QWidget* parent = 0);
		const Board::GameSetup& setup();
		void accept();
};
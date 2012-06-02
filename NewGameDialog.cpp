#include "NewGameDialog.hpp"
#include "BallTypeSettingsDialog.hpp"
#include "PlayerSetupWidget.hpp"
#include "ball.hpp"
#include "board.hpp"

#include <algorithm>

#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QIntValidator>
#include <QDialogButtonBox>
 
NewGameDialog::NewGameDialog(const Board::GameSetup& setup, QWidget* parent)
	: QDialog(parent), setupVal(setup), ballSettings(setup.ballTypeSettings)
{
	QVBoxLayout* leftLayout = new QVBoxLayout;
	QVBoxLayout* rightLayout = new QVBoxLayout;
	
	QIntValidator* validator = new QIntValidator(4, 30, this);
	rowLengthValidator = new QIntValidator(3, std::min(setup.width, setup.height), this);
	
	
	widthEdit = new QLineEdit(QString::number(setup.width), this);
	widthEdit->setValidator(validator);
	leftLayout->addWidget(new QLabel("Szerokosc planszy", this));
	leftLayout->addWidget(widthEdit);
	
	heightEdit = new QLineEdit(QString::number(setup.height), this);
	heightEdit->setValidator(validator);
	leftLayout->addWidget(new QLabel("Wysokosc planszy", this));
	leftLayout->addWidget(heightEdit);
	
	colorsEdit = new QLineEdit(QString::number(setup.colors), this);
	colorsEdit->setValidator(validator);
	leftLayout->addWidget(new QLabel("Liczb kolorow", this));
	leftLayout->addWidget(colorsEdit);
	
	rowLengthEdit = new QLineEdit(QString::number(setup.rowLength), this);
	rowLengthEdit->setValidator(rowLengthValidator);
	leftLayout->addWidget(new QLabel("Dlugosc rzadka kulek", this));
	leftLayout->addWidget(rowLengthEdit);
	
	roundLimitEdit = new QLineEdit(QString::number(setup.roundLimit), this);
	roundLimitEdit->setValidator(new QIntValidator(0, 1000000, this));
	leftLayout->addWidget(new QLabel("Limit rund (0 - bez limitu)", this));
	leftLayout->addWidget(roundLimitEdit);
	
	#warning players
	//~ for (int i = 0; i < 
	//~ QList<PlayerSetupWidget*> players;
	//~ BallTypeSettingsDialog* ballTypeSettingsDialog;
	

	QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Cancel);
		
	QPushButton* ballTypeSettingsButton = new QPushButton("Ustawienia bonusow", this);
	buttonBox->addButton(ballTypeSettingsButton, QDialogButtonBox::ActionRole);
	connect(ballTypeSettingsButton, SIGNAL(clicked()), this, SLOT(showBallTypeSettings()));
	
	QVBoxLayout* mainLayout = new QVBoxLayout;
	QHBoxLayout* splitLayout = new QHBoxLayout;
	
	//~ QWidget* leftPanel = new QWidget(this);
	//~ QWidget* rightPanel = new QWidget(this);
	splitLayout->addLayout(leftLayout);
	splitLayout->addLayout(rightLayout);
	
	mainLayout->addLayout(splitLayout);
	mainLayout->addWidget(buttonBox);
	
	setLayout(mainLayout);
	
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

const Board::GameSetup& NewGameDialog::setup()
{
	return setupVal;
}

void NewGameDialog::accept()
{
	setupVal.width = widthEdit->text().toInt();
	setupVal.height = heightEdit->text().toInt();
	setupVal.colors = colorsEdit->text().toUInt();
	setupVal.rowLength = rowLengthEdit->text().toUInt();
	setupVal.roundLimit = roundLimitEdit->text().toUInt();
	setupVal.ballTypeSettings = ballSettings;
	
	#warning setupVal.players = ??
	//~ setupVal.players = 
	
	setupVal.setAsDefault();
	
	QDialog::accept();
}

void NewGameDialog::showBallTypeSettings()
{
	BallTypeSettingsDialog dialog(ballSettings, this);
	if (dialog.exec() == QDialog::Accepted){
		ballSettings = dialog.settings();
	}
}

void NewGameDialog::playerWidgetDeleted()
{
	for (QList<PlayerSetupWidget*>::iterator i = players.begin(); i != players.end(); ++i){
		if (*i == dynamic_cast<PlayerSetupWidget*>(sender())){
			players.erase(i);
			return;
		}
	}
}
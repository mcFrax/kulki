#include "NewGameDialog.hpp"
#include "BallTypeSettingsDialog.hpp"
#include "PlayerSetupWidget.hpp"
#include "ball.hpp"
#include "board.hpp"

#include <algorithm>
#include <cstdlib>

#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QIntValidator>
#include <QDialogButtonBox>
#include <QListIterator>
#include <QScrollArea>

#include "debugtools.hpp"

NewGameDialog::NewGameDialog(const Board::GameSetup& setup, QWidget* parent)
	: QDialog(parent), setupVal(setup), ballSettings(setup.ballTypeSettings)
{
	QVBoxLayout* leftLayout = new QVBoxLayout;
	QVBoxLayout* rightLayout = new QVBoxLayout;
	
	QIntValidator* validator = new QIntValidator(4, 100, this);
	rowLengthValidator = new QIntValidator(3, std::min(setup.width, setup.height), this);
	
	widthEdit = new QLineEdit(QString::number(setup.width), this);
	widthEdit->setValidator(validator);
	leftLayout->addWidget(new QLabel("Szerokosc planszy", this));
	leftLayout->addWidget(widthEdit);
	connect(widthEdit, SIGNAL(textChanged(const QString&)), this, SLOT(updateRowLengthValidator()));
	
	heightEdit = new QLineEdit(QString::number(setup.height), this);
	heightEdit->setValidator(validator);
	leftLayout->addWidget(new QLabel("Wysokosc planszy", this));
	leftLayout->addWidget(heightEdit);
	connect(heightEdit, SIGNAL(textChanged(const QString&)), this, SLOT(updateRowLengthValidator()));
	
	colorsEdit = new QLineEdit(QString::number(setup.colors), this);
	colorsEdit->setValidator(new QIntValidator(2, 20, this));
	leftLayout->addWidget(new QLabel("Liczba kolorow", this));
	leftLayout->addWidget(colorsEdit);
	
	rowLengthEdit = new QLineEdit(QString::number(setup.rowLength), this);
	rowLengthEdit->setValidator(rowLengthValidator);
	leftLayout->addWidget(new QLabel("Dlugosc rzadka kulek", this));
	leftLayout->addWidget(rowLengthEdit);
	
	roundLimitEdit = new QLineEdit(QString::number(setup.roundLimit), this);
	roundLimitEdit->setValidator(new QIntValidator(0, 1000000, this));
	leftLayout->addWidget(new QLabel("Limit rund (0 - bez limitu)", this));
	leftLayout->addWidget(roundLimitEdit);
	
	leftLayout->addStretch();
	
	QListIterator<Player::PlayerInfo> iter(setup.players);
	while (iter.hasNext()){
		PlayerSetupWidget* p = new PlayerSetupWidget(iter.next(), this);
		rightLayout->addWidget(p);
		players.append(p);
		connect(p, SIGNAL(destroyed()), this, SLOT(playerWidgetDeleted()));
	}
	
	rightLayout->addStretch();
	QPushButton* addPlayerButton = new QPushButton(tr("Dodaj gracza"), this);
	rightLayout->addWidget(addPlayerButton);
	connect(addPlayerButton, SIGNAL(clicked()), this, SLOT(addPlayer()));
	
	playersLayout = rightLayout;

	QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Cancel);
		
	QPushButton* ballTypeSettingsButton = new QPushButton("Ustawienia bonusow", this);
	buttonBox->addButton(ballTypeSettingsButton, QDialogButtonBox::ActionRole);
	connect(ballTypeSettingsButton, SIGNAL(clicked()), this, SLOT(showBallTypeSettings()));
	
	okButton = buttonBox->button(QDialogButtonBox::Ok);
	
	okButton->setEnabled(!players.empty());
	
	QVBoxLayout* mainLayout = new QVBoxLayout;
	QHBoxLayout* splitLayout = new QHBoxLayout;
	
	QWidget* rightPanel = new QWidget(this);
	QScrollArea* scrollArea = new QScrollArea(this);
	scrollArea->setWidgetResizable(1);
	rightPanel->setLayout(rightLayout);
	scrollArea->setWidget(rightPanel);
	
	splitLayout->addLayout(leftLayout);
	splitLayout->addWidget(scrollArea, 1);
	
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
	
	setupVal.players.clear();
	QListIterator<PlayerSetupWidget*> iter(players);
	while (iter.hasNext()){
		setupVal.players.append(iter.next()->playerInfo());
	}
	
	setupVal.setAsDefault();
	
	QDialog::accept();
}

void NewGameDialog::updateRowLengthValidator()
{
	rowLengthValidator->setTop(std::min(widthEdit->text().toInt(), 
			heightEdit->text().toInt()));
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
		if (*i == static_cast<PlayerSetupWidget*>(sender())){
			players.erase(i);
			break;
		}
	}
	okButton->setEnabled(!players.empty());
}

void NewGameDialog::addPlayer()
{
	PlayerSetupWidget* p = new PlayerSetupWidget(
			Player::PlayerInfo(
				tr("Gracz ")+QString::number(players.size()+1), 
				Player::human, 
				QColor::colorNames()[rand()%QColor::colorNames().size()]), 
			this);
	
	connect(p, SIGNAL(destroyed()), this, SLOT(playerWidgetDeleted()));
	players.append(p);
	playersLayout->insertWidget(playersLayout->count()-2, p);
	okButton->setEnabled(!players.empty());
}
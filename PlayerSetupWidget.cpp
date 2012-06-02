#include <QComboBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QIcon>

#include "PlayerSetupWidget.hpp"
#include "ColorComboBox.hpp"
#include "player.hpp"

QLineEdit* nameEdit;

PlayerSetupWidget::PlayerSetupWidget(const Player::PlayerInfo& info, QWidget* parent)
{
	QVBoxLayout* layout = new QVBoxLayout();
	
	nameEdit = new QLineEdit(info.name, this);
	layout->addWidget(nameEdit);
	
	ColorComboBox* colorBox = new ColorComboBox(info.color, this);
	layout->addWidget(colorBox);
	
	QComboBox* typeBox = new QComboBox(this);
	for (int i = 0; i < Player::playerTypes; ++i)
		typeBox->addItem(Player::playerTypeNames[i]);
	layout->addWidget(typeBox);
	
	QPushButton* delButton = new QPushButton(QIcon(":DeletePlayerIcon.png"), QString());
	layout->addWidget(delButton);
	
	connect(delButton, SIGNAL(clicked()), this, SLOT(deleteLater()));
		
	setLayout(layout);
}

Player::PlayerInfo PlayerSetupWidget::playerInfo()
{
	return Player::PlayerInfo(nameEdit->text(), typeBox->currentIndex(), colorBox->color());
}
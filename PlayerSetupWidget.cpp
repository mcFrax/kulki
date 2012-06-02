#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QIcon>

#include "PlayerSetupWidget.hpp"
#include "ColorComboBox.hpp"
#include "player.hpp"


PlayerSetupWidget::PlayerSetupWidget(const Player::PlayerInfo& info, QWidget* parent)
{
	QHBoxLayout* layout = new QHBoxLayout();
	
	nameEdit = new QLineEdit(info.name, this);
	layout->addWidget(nameEdit);
	
	colorBox = new ColorComboBox(info.color, this);
	layout->addWidget(colorBox);
	
	typeBox = new QComboBox(this);
	for (int i = 0; i < Player::playerTypes; ++i)
		typeBox->addItem(Player::playerTypeNames[i]);
	layout->addWidget(typeBox);
	typeBox->setCurrentIndex(info.type);
	
	QPushButton* delButton = new QPushButton(QIcon(":DeletePlayerIcon.png"), tr("Usun"));
	layout->addWidget(delButton);
	
	connect(delButton, SIGNAL(clicked()), this, SLOT(deleteLater()));
		
	setLayout(layout);
}

Player::PlayerInfo PlayerSetupWidget::playerInfo()
{
	return Player::PlayerInfo(nameEdit->text(), typeBox->currentIndex(), colorBox->color());
}
#pragma once

#include <QWidget>

#include "player.hpp"

class QComboBox;
class ColorComboBox;

class PlayerSetupWidget : public QWidget
{
	private:
		QLineEdit* nameEdit;
		ColorComboBox* colorBox;
		QComboBox* typeBox;
	public:
		PlayerSetupWidget(const Player::PlayerInfo& info, QWidget* parent = 0);
		Player::PlayerInfo playerInfo();
};
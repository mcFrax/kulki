#pragma once

#include <QDialog>
#include <QList>

#include "ball.hpp"

class QLineEdit;

class BallTypeSettingsDialog : public QDialog
{
	private:
		QLineEdit** edits;
		QList<uint> settingsVal;
	public:
		BallTypeSettingsDialog(const QList<uint> settings, QWidget* parent = 0);
		~BallTypeSettingsDialog();
		void accept();
		const QList<uint>& settings() const;
};
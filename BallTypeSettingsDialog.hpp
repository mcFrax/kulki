#pragma once

#include <QDialog>
#include <QList>

#include "ball.hpp"

class QLineEdit;

class BallTypeSettingsDialog : public QDialog
{
	Q_OBJECT
	private:
		QLineEdit** edits;
		QList<uint> settingsVal;
		QLineEdit* factorEdit;
	private slots:
		void updateFactor();
	public:
		BallTypeSettingsDialog(const QList<uint> settings, QWidget* parent = 0);
		~BallTypeSettingsDialog();
		void accept();
		const QList<uint>& settings() const;
};
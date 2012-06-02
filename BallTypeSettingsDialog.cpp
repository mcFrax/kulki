#include "BallTypeSettingsDialog.hpp"

#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QIntValidator>
#include <QDialogButtonBox>

BallTypeSettingsDialog::BallTypeSettingsDialog(const QList<uint> settings, QWidget* parent)
	: QDialog(parent), settingsVal(settings)
{
	edits = new QLineEdit*[Ball::specialBallTypes];
	QVBoxLayout* layout = new QVBoxLayout;
	for (int i = 0; i < Ball::specialBallTypes; ++i){
		//~ if (settingsVal.size() <= i) settingsVal.append(0); //ew. zabezpieczenie
		edits[i] = new QLineEdit(QString::number(settingsVal[i]), this);
		edits[i]->setValidator(new QIntValidator(0, 1000/Ball::specialBallTypes, this));
		layout->addWidget(new QLabel(Ball::specialBallTypeNames[i], this));
		layout->addWidget(edits[i]);
	}

	QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Cancel);
	layout->addWidget(buttonBox);
	
	setLayout(layout);

	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

BallTypeSettingsDialog::~BallTypeSettingsDialog()
{
	delete [] edits;
}

void BallTypeSettingsDialog::accept()
{
	for (int i = 0; i < Ball::specialBallTypes; ++i){
		settingsVal[i] = edits[i]->text().toUInt();
	}
	QDialog::accept();
}

const QList<uint>& BallTypeSettingsDialog::settings() const
{
	return settingsVal;
}
#include <QList>
#include <QPair>
#include <QLabel>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QtAlgorithms>

#include "HighScores.hpp"
#include "settings.hpp"
#include "player.hpp"
#include "ball.hpp"
#include "main_window.hpp"


void recordHighScore(Player* p, const Board::GameSetup& s)
{
	qreal realPoints = p->points() * Ball::levelFactor(s.ballTypeSettings);
	QList<QVariant> varList = settings()->value("highscores").toList();
	QList<QPair<qreal, QString> > list;
	for (int i = 0; i < varList.size()-1; i+=2)
		list.append(qMakePair(varList.at(i).toDouble(), varList.at(i+1).toString()));
	
	qSort(list);
	
	if (list.size() >= 10 && realPoints <= list[0].first)
		return;
		
	if (list.size() >= 10) list.removeFirst();
	QList<QPair<qreal, QString> >::iterator it = list.begin();
	while(it != list.end() && it->first < realPoints)
		++it;
	list.insert(it, qMakePair(realPoints, p->name()));
	
	varList.clear();
	for (int i = 0; i < list.size(); ++i){
		varList.append(list.at(i).first);
		varList.append(list.at(i).second);
	}
	
	settings()->setValue("highscores", varList);
	
	showHighScores();
}

static QWidget* recordWidget(qreal points, QString name, QWidget* parent)
{
	QWidget* res = new QWidget(parent);
	QHBoxLayout* layout = new QHBoxLayout();
	layout->addWidget(new QLabel(name, res));
	layout->addStretch();
	layout->addWidget(new QLabel(QString::number(points, 'f', 2), res));
	res->setLayout(layout);
	return res;
}

void showHighScores()
{
	QDialog* dialog = new QDialog(MainWindow::instance());
	QVBoxLayout* layout = new QVBoxLayout();
	layout->addWidget(new QLabel("Najlepsze wyniki", dialog));
	
	const QList<QVariant> varList = settings()->value("highscores").toList();
	for (int i = varList.size()-2; i >= 0; i-=2)
		layout->addWidget(recordWidget(varList.at(i).toDouble(), varList.at(i+1).toString(), dialog));
		
	layout->addStretch();
	
	QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);
	buttonBox->setCenterButtons(1);
	layout->addWidget(buttonBox);
	
	dialog->setLayout(layout);

	QObject::connect(buttonBox, SIGNAL(accepted()), dialog, SLOT(accept()));
	
	dialog->exec();
	
	delete dialog;
}
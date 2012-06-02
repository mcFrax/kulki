#include <QPoint>
#include <QAbstractAnimation>
#include <QListIterator>

#include <ostream>

#include "board.hpp"
#include "boardimplementation.hpp"
#include "square.hpp"
#include "ballcolor.hpp"
#include "player.hpp"

#include "settings.hpp"
#include "debugtools.hpp"

using namespace std;

std::ostream& operator << (std::ostream& os, Board::State s){
	switch (s) {
		case Board::waitingForPlayer:
			return os << "waitingForPlayer";
		case Board::waitingForMove:
			return os << "waitingForMove";
		case Board::animatingMove:
			return os << "animatingMove";
		case Board::locked:
			return os << "locked";
		default:
			return os;
	}
}

//!Fabryka
Board* Board::newBoard(const GameSetup& s, QGraphicsItem * parent)
{
	return new BoardImplementation(s, parent);
}

static inline qreal margin()
{
	return settings()->value("board/margin").toDouble();
}

//!Konstruktor
Board::Board(const GameSetup& s, QGraphicsItem * parent)
	: QGraphicsRectItem(
			-margin(), -margin(),
			s.width*Square::size() + 2*margin(),
			s.height*Square::size() + 2*margin(),
			parent), 
		setup(s), state(animatingMove), internalState(normal),
		curPlayer(0), nextPlayer(0), squares(s.width, s.height)
{
}

//!Zwraca biezacego gracza
Player* Board::currentPlayer()
{
	return curPlayer;
}

//!Zwraca nastepnego gracza (jezeli zostal ustawiony)
Player* Board::getNextPlayer()
{
	return nextPlayer;
}

//!Zwraca gameSetup
const Board::GameSetup& Board::gameSetup()
{
	return setup;
}

//!Zwraca aktualny stan planszy/gry
Board::State Board::getState()
{
	return state;
}

//!Ustawia state i emituje stateChanged (gdy state != s)
void Board::setState(State s)
{
	if (state == s) return;
	state = s;
	//~ PRINT(state);
	emit stateChanged(state);
	if (s == waitingForMove && curPlayer){
		//~ std::cerr << "Ruch gracza " << curPlayer->name().toStdString() << endl;
		curPlayer->makeMove(this);
		emit playerTurn(curPlayer);
	}
}

//!Sprawdza, czy wspolrzedne (x, y) mieszcza sie w planszy
bool Board::inBoard(int x, int y)
{
	return x >= 0 && x < setup.width
		&& y >= 0 && y < setup.height;
}

//!Sprawdza, czy ruch - zamiana s1 z s2 - jest legalny
bool Board::isLegal(Square* s1, Square* s2) const
{
	return legalMoves.find(make_pair(s1, s2)) != legalMoves.end();
}

//!Sprawdza, czy ruch - zamiana p1 z p2 - jest legalny
bool Board::isLegal(QPoint p1, QPoint p2) const
{
	return isLegal(squares(p1.x(), p1.y()), squares(p2.x(), p2.y()));
}

//!Wykonuje ruch - zamiane p1 z p2
bool Board::move(QPoint p1, QPoint p2)
{
	return move(squares(p1.x(), p1.y()), squares(p2.x(), p2.y()));
}

//!Rejestruje animacje jako trwajaca
void Board::registerAnimation(QAbstractAnimation* anim)
{
	currentAnimations.insert(anim);
	connect(anim, SIGNAL(finished()), this, SLOT(animationEnded()));
}

//!Wywolywana przez konczaca sie animacje
void Board::animationEnded()
{
	set<QObject*>::iterator i = currentAnimations.find(sender());
	if (i == currentAnimations.end())
		return;
	currentAnimations.erase(i);
	
	//to nie jest najladniejsze rozwiazanie, pewnie moglbym uzyc QStateMachine
	//ale teraz juz nie bede zmienial
	if (currentAnimations.empty()){
		switch (internalState) {
			case normal: 
				check();
				return;
			case falling:
				refill();
				return;
		}
	}
}

Q_DECLARE_METATYPE(QList<uint>)

Board::GameSetup::GameSetup()
	:	width(settings()->value("game settings/width").toUInt()),
		height(settings()->value("game settings/height").toUInt()),
		colors(settings()->value("game settings/colors").toUInt()),
		rowLength(settings()->value("game settings/rowLength").toInt()),
		players(Player::createPlayers(settings()->value("game settings/players").toList())),
		ballTypeSettings(settings()->value("game settings/ballTypeSettings").value<QList<uint> >()),
		roundLimit(settings()->value("game settings/roundLimit").toUInt())
{
}

static QVariant convertBTS(const QList<uint> ls)
{
	QList<QVariant> res;
	QListIterator<uint> iterator(ls);
	while (iterator.hasNext())
		res.append(iterator.next());
	return res;
}

void Board::GameSetup::setAsDefault()
{
	settings()->setValue("game settings/width", width);
	settings()->setValue("game settings/height", height);
	settings()->setValue("game settings/colors", colors);
	settings()->setValue("game settings/rowLength", rowLength);
	settings()->setValue("game settings/players", Player::toQVariantList(players));
	settings()->setValue("game settings/ballTypeSettings", convertBTS(ballTypeSettings));
	settings()->setValue("game settings/roundLimit", roundLimit);
}

//!Kontruktor
Board::BoardInfo::BoardInfo(Square** array, int width, int height)
	: array(array), arrayWidth(width), arrayHeight(height)
{
}

//!Square na pozycji (x, y)
BallColor Board::BoardInfo::operator () (int x, int y) const
{
	return array[y*arrayWidth+x]->ballColor();
}

//!Szerokosc (w polach)
int Board::BoardInfo::width() const
{
	return arrayWidth;
}

//!Wyskokosc (w polach)
int Board::BoardInfo::height() const
{
	return arrayHeight;
}

//!Sprawdza, czy wspolrzedne (x, y) mieszcza sie w planszy
bool Board::BoardInfo::contains(int x, int y) const
{
	return x >= 0 && x < arrayWidth
		&& y >= 0 && y < arrayHeight;
}

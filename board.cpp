#include <ostream>

#include "board.hpp"
#include "boardimplementation.hpp"
#include "square.hpp"
#include "ballcolor.hpp"

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
	}
}

//!Konstruktor
Board* Board::newBoard(const GameSetup& s, QObject * parent)
{
	return new BoardImplementation(s, parent);
}

//!Konstruktor
Board::Board(const GameSetup& s, QObject * parent)
	: QGraphicsScene(0, 0, s.width*Square::xSize,
		s.height*Square::ySize, parent), setup(s), state(animatingMove),
		curPlayer(0)
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
	PRINT(state);
	emit stateChanged(state);
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

//!Rejestruje animacje jako trwajaca
void Board::registerAnimation(QObject* anim)
{
	currentAnimations.insert(anim);
}

//!Wywolywana przez konczaca sie animacje
void Board::animationEnded()
{
	set<QObject*>::iterator i = currentAnimations.find(sender());
	if (i == currentAnimations.end())
		return;
	currentAnimations.erase(i);
	if (currentAnimations.empty()){
		check(0);
	}
}

//!Kontruktor
Board::BoardInfo::BoardInfo(Square** array, uint width, uint height)
	: array(array), arrayWidth(width), arrayHeight(height)
{
}

//!Square na pozycji (x, y)
BallColor Board::BoardInfo::operator () (uint x, uint y) const
{
	return array[y*arrayWidth+x]->ballColor();
}

//!Szerokosc (w polach)
uint Board::BoardInfo::width() const
{
	return arrayWidth;
}

//!Wyskokosc (w polach)
uint Board::BoardInfo::height() const
{
	return arrayHeight;
}

//!Sprawdza, czy wspolrzedne (x, y) mieszcza sie w planszy
bool Board::BoardInfo::contains(int x, int y) const
{
	return x >= 0 && x < arrayWidth
		&& y >= 0 && y < arrayHeight;
}

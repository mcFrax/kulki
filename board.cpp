#include <QGraphicsSceneMouseEvent>

#include "board.hpp"
#include "boardimplementation.hpp"
#include "square.hpp"
#include "ball.hpp"

#include "debugtools.hpp"

using namespace std;

//klasa do "udawanych" zamian - przy sprawdzaniu ruchow
//dziala identycznie z Board::BoardInfo, ale zamienia s1 z s2
//!Udaje Board::BoardInfo z zamiana dwoch Square'ow
class SwapPretender
{
	private:
		Square** array;
		uint arrayWidth;
		uint arrayHeight;
		Square* s1;
		Square* s2;
	public:
		SwapPretender(Square** array, uint arrayWidth, uint arrayHeight,
				Square* s1, Square* s2)
			: array(array), arrayWidth(arrayWidth), 
					arrayHeight(arrayHeight), s1(s1), s2(s2)
		{
		}
		BallColor operator () (uint x, uint y) const
		{
			Square* square = array[y*arrayWidth+x];
			if (square == s1)
				square = s2;
			else if (square == s2)
				square = s1;
			return square->ballColor();
		}
		uint width() const
		{
			return arrayWidth;
		}
		uint height() const
		{
			return arrayHeight;
		}
		bool contains(int x, int y) const
		{
			return x >= 0 && x < arrayWidth
				&& y >= 0 && y < arrayHeight;
		}
};


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

//!Rejestruje animacje onim jako trwajaca
void Board::registerAnimation(QObject* anim)
{
	currentAnimations.insert(anim);
}

//!Wywolywany przez konczaca sie animacje
void Board::animationEnded()
{
	set<QObject*>::iterator i = currentAnimations.find(sender());
	if (i == currentAnimations.end())
		return;
	currentAnimations.erase(i);
	if (currentAnimations.empty()){
		check();
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

//!Konstruktor
BoardImplementation::BoardImplementation(const GameSetup& s, QObject * parent)
	: Board(s, parent)
{
	BallColor::createTable(setup.colors);
	squares = new Square*[s.width*s.height];
	
	//creating squares
	for (uint iy = 0; iy < s.height; ++iy){
		for (uint ix = 0; ix < s.width; ++ix){
			squares[iy*s.width+ix] = new Square(ix, iy, this);
		}
	}
	
	//setting squares neighbours
	for (uint iy = 0; iy < s.height; ++iy){
		for (uint ix = 0; ix < s.width; ++ix){
			square(ix, iy)->setNeighbours(
					ix == 0 ? 0 : square(ix-1, iy),
					iy == 0 ? 0 : square(ix, iy-1),
					ix == s.width-1  ? 0 : square(ix+1, iy),
					iy == s.height-1 ? 0 : square(ix, iy+1)
					);
		}
	}
	
	//placing balls
	#warning here must go avoiding triplets on start
	#warning possibly by making continous fall till its good
	for (uint iy = 0; iy < s.height; ++iy){
		for (uint ix = 0; ix < s.width; ++ix){
			Ball::getNew(setup, square(ix, iy), setup.height+2);
		}
	}
	
	computeLegalMoves();
	setState(animatingMove);
}

//!Destruktor
BoardImplementation::~BoardImplementation()
{
	delete squares;
}

//!Punkty za rzadek
int BoardImplementation::Row::points() const
{
	int res = 0;
	for (const_iterator i = begin(); i != end(); ++i)
		res += (*i)->getPointValue();
	for (const_iterator i = begin(); i != end(); ++i)
		res = (*i)->applyPointModificator(res);
	return res;
}

//!Sprawdza, czy b pasuje do kulek w tym rzadku
bool BoardImplementation::Row::matches(Ball* b) const
{
	for (const_iterator i = begin(); i != end(); ++i)
		if ((*i)->getColor() != b->getColor())
			return 1;
	return empty();
}

//!overrides QGraphicsScene::mousePressEvent
void BoardImplementation::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	switch (state) {
		case waitingForMove :
			QGraphicsScene::mousePressEvent(event);
			return;
		default:
			event->ignore();
	}
}

//!overrides QGraphicsScene::mouseMoveEvent
void BoardImplementation::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	switch (state) {
		case waitingForMove :
			QGraphicsScene::mouseMoveEvent(event);
			return;
		default:
			event->ignore();
	}
}

//!Wykonanie ruchu - zamiany kulki na s1 z kulka na s2 - przez biezacego gracza
bool BoardImplementation::move(Square* s1, Square* s2)
{
	if (state != waitingForMove || !isLegal(s1, s2))
		return 0;
	setState(animatingMove);
	Ball* b1 = s1->getBall();
	Ball* b2 = s2->getBall();
	b1->placeOnSquare(s2);
	b2->placeOnSquare(s1);
	
	check();
	return 1;
}

//dzieki template'owi moge uzywac i BoardInfo i SwapPretender,
//bez polimorfizmu i bez copy'iego paste'a
//!zlicza ile jest kulek na (sX,sY) i sasiednich polach typu (sX+k*xM, sY+k*yM) koloru bc
template <class BoardInfo_or_SwapPretender>
uint countSame(uint sX, uint sY, bool xM, bool yM, BallColor bc,
		const BoardInfo_or_SwapPretender& board)
{
	if (board(sX, sY) != bc)
		return 0;
	
	uint res = 1;
	int ix = sX+xM;
	int iy = sY+yM;
	while (board.contains(ix, iy) && board(ix, iy) == bc){
		++res;
		ix += xM;
		iy += yM;
	}
	ix = sX-xM;
	iy = sY-yM;
	while (board.contains(ix, iy) && board(ix, iy) == bc){
		++res;
		ix -= xM;
		iy -= yM;
	}
	return res;
}

//!Sprawdza, czy jest jakas trojka (rzad), zawierajacy kulke na (x,y)
template <class BoardInfo_or_SwapPretender>
static bool checkForRow(uint x, uint y, const Board::GameSetup& setup,
		const BoardInfo_or_SwapPretender& board)
{
	for (uint c = 0; c < setup.colors; ++c){
		if (countSame(x, y, 1, 0, c, board) >= setup.rowLength
				|| countSame(x, y, 0, 1, c, board) >= setup.rowLength)
			return 1;
	}
	return 0;
}

//!Wylicza legalne zamiany typu <x,y> z <x+dx, y+dy>
void BoardImplementation::computeLegalMoves(const int dx, const int dy)
{
	for ( uint iy = 0; iy < setup.height-dy; ++iy){
		for ( uint ix = 0; ix < setup.width-dx; ++ix){
			//symulacja zamiany, i sprawdzenie, czy cos daje
			SwapPretender pret(squares, setup.width, setup.height, 
					square(ix, iy), square(ix+dx, iy+dy));
			if (checkForRow(ix, iy, setup, pret)
					|| checkForRow(ix+dx, iy+dy, setup, pret)){
				legalMoves.insert(make_pair(square(ix, iy), 
						square(ix+dx, iy+dy)));
				legalMoves.insert(make_pair(square(ix+dx, iy+dy), 
						square(ix, iy)));
			}
		}
	}
}

//!Wylicza wszystkie legalne zamiany
bool BoardImplementation::computeLegalMoves()
{
	legalMoves.clear();
	computeLegalMoves(1, 0);
	computeLegalMoves(0, 1);
	return !legalMoves.empty();
}

//!Square na pozycji (x, y)
Square* BoardImplementation::square(uint x, uint y)
{
	return squares[y*setup.width+x];
}

//!Rozpoczyna nowa ture
void BoardImplementation::newPly()
{
	total = 0;
	curPlayer = nextPlayer;
	nextPlayer = 0;
	if (curPlayer){
		setState(waitingForMove);
	} else {
		setState(waitingForPlayer);
	}
}

BoardImplementation::Rows BoardImplementation::findRows()
{
	#warning teraz jest glupio i zle, bo jedna kulka moze byc tylko w jednym Row`ie (w kazdym z kierunkow)
	Rows res;
	//zliczanie poziomo
	Row curRow;
	for (uint iy = 0; iy < setup.height-setup.rowLength; ++iy){
		for (uint ix = 0; ix < setup.width-setup.rowLength; ++ix){
			Ball* b = square(ix, iy)->getBall();
			if (curRow.matches(b)){
				curRow.push_back(b);
			} else {
				if (curRow.size() >= setup.rowLength){
					res.push_back(Row());
					swap(res.back(), curRow);
				} else {
					curRow = Row();
				}
			}
		}
	}
	//zliczanie pionowo
	for (uint ix = 0; ix < setup.width-setup.rowLength; ++ix){
		for (uint iy = 0; iy < setup.height-setup.rowLength; ++iy){
			
		}
	}
	return res;
}

//!Sprawdzenie planszy (po ruchu/uzupelnieniu) i odpowiednia zmiana stanu
void BoardImplementation::check()
{
	setState(animatingMove);
	const Rows& rows = findRows();
	if (rows.empty()){
		//spadanie sie skonczylo
		emit playerMoveEnded(curPlayer, total);
		if (!computeLegalMoves())
			setState(locked);
		else
			newPly();
	} else {
		set<Ball*> balls;
		for (Rows::const_iterator ir = rows.begin(); ir != rows.end(); ++ir){
			for (Row::const_iterator ib = ir->begin(); ib != ir->end(); ++ib)
				balls.insert(*ib);
			total += ir->points();
			
		}
		for (set<Ball*>::const_iterator i = balls.begin(); i != balls.end(); ++i)
			delete *i;
		refill();
	}
}

//!zepchniecie kulek (grawitacja) + utworzenie nowych (zapelnienie planszy)
void BoardImplementation::refill()
{
	for ( int iy = setup.height-1; iy >= 0 ; --iy){
		for ( int ix = 0; ix < setup.width; ++ix){
			square(ix, iy)->takeBall();
		}
	}
}

#warning more documentation needed here (or there)
//!Oddaje nastepny ruch graczowi p.
void BoardImplementation::setCurrentPlayer(Player* p)
{
	if (state == waitingForPlayer){
		curPlayer = p;
		if (state == waitingForPlayer)
			setState(waitingForMove);
	} else if (!nextPlayer) {
		nextPlayer = p;
	}
}